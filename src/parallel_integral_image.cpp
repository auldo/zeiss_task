#include "parallel_integral_image.h"

cv::Mat computeIIParallel(const cv::Mat& input, int gridSize) {

    // Initialize output mat with same size as input mat
    cv::Mat output(input.rows, input.cols, CV_16UC1);

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
                            cumulativeRowSums.at(c) += input.at<uint16_t>(r, c);
                            lastIIValue = cumulativeRowSums.at(c) + lastIIValue;
                            output.at<uint16_t>(r, c) = lastIIValue;
                        }
                    }
                }
            });
        }

        // Join the threads again.
        for (auto& thread : threads)
            thread.join();
    }

    // Initialize containers for VBP and HBP
    cv::Mat verticalBorderPixels(segY, input.cols, CV_16UC1);

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
                        verticalBorderPixels.at<uint16_t>(y, x) = output.at<uint16_t>(std::min(y * gridSize + gridSize - 1, input.rows - 1), x) + (y > 0 ? verticalBorderPixels.at<uint16_t>(y - 1, x) : 0);
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