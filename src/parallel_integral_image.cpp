#include "parallel_integral_image.h"

cv::Mat computeIIParallel(const cv::Mat& input, int gridSize) {

    // Initialize output mat with same size as input mat
    cv::Mat output(input.rows, input.cols, II_CONST_CV_TYPE);

    // Calculate x and y direction counts for the segmentation count
    // Avoid failure on odd grid size
    const int segX{input.cols % gridSize == 0 ? input.cols / gridSize : input.cols / gridSize + 1};
    const int segY{input.rows % gridSize == 0 ? input.rows / gridSize : input.rows / gridSize + 1};

    {
        // Create list of segmentation jobs
        // A segmentation job is described by the starting point of the segment
        // A segmentation job results in the segment having a locally applied integral image algorithm
        std::mutex segmentationJobMutex;
        std::stack<std::pair<int, int>> segmentationJobs;
        for (int rowIndex{0}; rowIndex < segY; ++rowIndex) {
            for (int colIndex{0}; colIndex < segX; ++colIndex) {
                segmentationJobs.emplace(rowIndex * gridSize, colIndex * gridSize);
            }
        }

        // Create worker threads for the segmentation jobs and run them
        // Taking jobs from the queue must be synchronized to avoid data races
        std::vector<std::thread> threads;
        const auto threadCount{std::min(std::thread::hardware_concurrency() - 1, static_cast<unsigned int>(segmentationJobs.size()))};
        for (int i{0}; i < threadCount; ++i) {
            threads.emplace_back([&] {
                while (true) {
                    std::unique_lock l{segmentationJobMutex};
                    if (segmentationJobs.empty()) {
                        l.unlock();
                        break;
                    }
                    // Takes a job from the queue

                    auto [x, y] = segmentationJobs.top();
                    segmentationJobs.pop();
                    l.unlock();

                    // The sequential algorithm applied locally to the segment
                    const int cols = std::min(output.cols, x + gridSize);
                    const int rows = std::min(output.rows, y + gridSize);

                    std::vector<unsigned int> cumulativeRowSums(output.cols);
                    for(int r{y}; r < rows; ++r) {
                        unsigned int lastIIValue{0};
                        for(int c{x}; c < cols; ++c) {
                            cumulativeRowSums.at(c) += input.at<II_CONST_COMP_TYPE>(r, c);
                            lastIIValue = cumulativeRowSums.at(c) + lastIIValue;
                            output.at<II_CONST_COMP_TYPE>(r, c) = lastIIValue;
                        }
                    }
                }
            });
        }

        // Join the threads again.
        for (auto& thread : threads)
            thread.join();
    }

    // Initialize containers for VBP
    cv::Mat verticalBorderPixels(segY, input.cols, II_CONST_CV_TYPE);

    {
        std::mutex vbpJobMutex{};
        std::stack<int> vbpJobs;
        for (int i{0}; i < input.cols; ++i) {
            vbpJobs.emplace(i);
        }

        const auto threadCount{std::min(std::thread::hardware_concurrency() - 1, static_cast<unsigned int>(vbpJobs.size()))};
        std::vector<std::thread> threads;

        for (auto i{0}; i < threadCount; ++i) {
            threads.emplace_back([&] {
                while (true) {
                    std::unique_lock l{vbpJobMutex};
                    if (vbpJobs.empty()) {
                        l.unlock();
                        break;
                    }
                    const int x = vbpJobs.top();
                    vbpJobs.pop();
                    l.unlock();
                    for (int y{0}; y < segY; ++y) {
                        verticalBorderPixels.at<II_CONST_COMP_TYPE>(y, x) = output.at<II_CONST_COMP_TYPE>(std::min(y * gridSize + gridSize - 1, input.rows - 1), x) + (y > 0 ? verticalBorderPixels.at<II_CONST_COMP_TYPE>(y - 1, x) : 0);
                    }
                }
            });
        }

        // Join the threads again.
        for (auto& thread : threads)
            thread.join();
    }

    // Initialize containers for HBP
    cv::Mat horizontalBorderPixels(input.rows, segX, II_CONST_CV_TYPE);

    {
        std::mutex hbpJobMutex{};
        std::stack<int> hbpJobs;
        for (int i{0}; i < input.rows; ++i) {
            hbpJobs.emplace(i);
        }

        const auto threadCount{std::min(std::thread::hardware_concurrency() - 1, static_cast<unsigned int>(hbpJobs.size()))};
        std::vector<std::thread> threads;

        for (auto i{0}; i < threadCount; ++i) {
            threads.emplace_back([&]() {
                while (true) {
                    std::unique_lock l{hbpJobMutex};
                    if (hbpJobs.empty()) {
                        l.unlock();
                        break;
                    }
                    int y{hbpJobs.top()};
                    hbpJobs.pop();
                    l.unlock();

                    for (int x{0}; x < segX; ++x) {
                        horizontalBorderPixels.at<II_CONST_COMP_TYPE>(y, x) = output.at<II_CONST_COMP_TYPE>(y, std::min(x * gridSize + gridSize - 1, input.cols - 1)) + (x > 0 ? horizontalBorderPixels.at<II_CONST_COMP_TYPE>(y, x - 1) : 0);
                    }
                }
            });
        }

        // Join the threads again.
        for (auto& thread : threads)
            thread.join();
    }

    // Initialize container for CBor
    cv::Mat rightBottomCorners(segY, segX, II_CONST_CV_TYPE);

    {
        std::mutex cborJobMutex{};
        std::stack<int> cborJobs;
        for (int i{0}; i < segX; ++i) {
            cborJobs.emplace(i);
        }

        const auto threadCount{std::min(std::thread::hardware_concurrency() - 1, static_cast<unsigned int>(cborJobs.size()))};
        std::vector<std::thread> threads;

        for (auto i{0}; i < threadCount; ++i) {
            threads.emplace_back([&]() {
                while (true) {
                    std::unique_lock l{cborJobMutex};
                    if (cborJobs.empty()) {
                        l.unlock();
                        break;
                    }
                    int x{cborJobs.top()};
                    cborJobs.pop();
                    l.unlock();

                    for (int y{0}; y < segY; ++y) {
                        rightBottomCorners.at<II_CONST_COMP_TYPE>(y, x) = horizontalBorderPixels.at<II_CONST_COMP_TYPE>(std::min(y * gridSize + gridSize - 1, horizontalBorderPixels.rows - 1), x) + (y > 0 ? rightBottomCorners.at<II_CONST_COMP_TYPE>(y - 1, x) : 0);
                    }
                }
            });
        }

        // Join the threads again.
        for (auto& thread : threads)
            thread.join();
    }

    // Update HBP
    {
        std::mutex hbpUpdateJobMutex{};
        std::stack<int> hbpUpdateJobs;
        for (int i{0}; i < segX; ++i) {
            hbpUpdateJobs.emplace(i);
        }

        const auto threadCount{std::min(std::thread::hardware_concurrency() - 1, static_cast<unsigned int>(hbpUpdateJobs.size()))};
        std::vector<std::thread> threads;

        for (auto i{0}; i < threadCount; ++i) {
            threads.emplace_back([&]() {
                while (true) {
                    std::unique_lock l{hbpUpdateJobMutex};
                    if (hbpUpdateJobs.empty()) {
                        l.unlock();
                        break;
                    }
                    int x{hbpUpdateJobs.top()};
                    hbpUpdateJobs.pop();
                    l.unlock();

                    for (int y{gridSize}; y < horizontalBorderPixels.rows; ++y) {
                        horizontalBorderPixels.at<II_CONST_COMP_TYPE>(y, x) += rightBottomCorners.at<II_CONST_COMP_TYPE>(y / gridSize - 1, x);
                    }
                }
            });
        }

        // Join the threads again.
        for (auto& thread : threads)
            thread.join();
    }

    // Global update
    {
        std::mutex segmentationJobMutex;
        std::stack<std::pair<int, int>> segmentationJobs;
        for (int rowIndex{0}; rowIndex < segY; ++rowIndex) {
            for (int colIndex{0}; colIndex < segX; ++colIndex) {
                segmentationJobs.emplace(rowIndex * gridSize, colIndex * gridSize);
            }
        }

        const auto threadCount{std::min(std::thread::hardware_concurrency() - 1, static_cast<unsigned int>(segmentationJobs.size()))};
        std::vector<std::thread> threads;

        for (auto i{0}; i < threadCount; ++i) {
            threads.emplace_back([&]() {
                while (true) {
                    std::unique_lock l{segmentationJobMutex};
                    if (segmentationJobs.empty()) {
                        l.unlock();
                        break;
                    }
                    const auto [x, y]{segmentationJobs.top()};
                    segmentationJobs.pop();
                    l.unlock();

                    const int cols = std::min(output.cols, x + gridSize);
                    const int rows = std::min(output.rows, y + gridSize);

                    if (y == 0 && x == 0)
                        continue;

                    if (y == 0) {
                        for (int r{y}; r < rows; ++r) {
                            for (int c{x}; c < cols; ++c) {
                                output.at<II_CONST_COMP_TYPE>(r, c) += horizontalBorderPixels.at<II_CONST_COMP_TYPE>(r, c / gridSize - 1);
                            }
                        }
                        continue;
                    }

                    if (x == 0) {
                        for (int r{y}; r < rows; ++r) {
                            for (int c{x}; c < cols; ++c) {
                                output.at<II_CONST_COMP_TYPE>(r, c) += verticalBorderPixels.at<II_CONST_COMP_TYPE>(r / gridSize - 1, c);
                            }
                        }
                        continue;
                    }

                    for (int r{y}; r < rows; ++r) {
                            for (int c{x}; c < cols; ++c) {
                                output.at<II_CONST_COMP_TYPE>(r, c) += verticalBorderPixels.at<II_CONST_COMP_TYPE>(r / gridSize - 1, c) + horizontalBorderPixels.at<II_CONST_COMP_TYPE>(r, c / gridSize - 1);
                            }
                        }
                }
            });
        }

        // Join the threads again.
        for (auto& thread : threads)
            thread.join();
    }

    return output;
}