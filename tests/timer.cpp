#include <iostream>

#include "integral_image.h"

cv::Mat rescaleOutput(const cv::Mat& output) {
    cv::Mat rescaled;
    cv::normalize(output, rescaled, 0, 255, cv::NORM_MINMAX, II_CONST_CV_TYPE);
    cv::Mat converted;
    rescaled.convertTo(converted, CV_8UC1);
    return converted;
}

void randomExample() {
    const auto example{integralImageInputExampleLargeRandom(2000, 1)};
    {
        const auto begin = std::chrono::steady_clock::now();
        const auto output{computeIISequential(example)};
        const auto end = std::chrono::steady_clock::now();
        std::cout << "time difference for sequential\t\t" << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms" << std::endl;
        const auto rescaled{rescaleOutput(output)};
        cv::imwrite("/Users/dominikaulinger/Desktop/sequential.jpg", rescaled);
    }

    {
        const auto begin = std::chrono::steady_clock::now();
        const auto output{computeIIParallel(example, 200)};
        const auto end = std::chrono::steady_clock::now();
        std::cout << "time difference for parallel\t\t" << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms" << std::endl;
        const auto rescaled{rescaleOutput(output)};
        cv::imwrite("/Users/dominikaulinger/Desktop/parallel.jpg", rescaled);
    }
}

void realExample() {
    auto image{cv::imread("/Users/dominikaulinger/Desktop/random.jpg", cv::IMREAD_GRAYSCALE)};
    image.convertTo(image, II_CONST_CV_TYPE);
    {
        const auto begin = std::chrono::steady_clock::now();
        const auto output{computeIISequential(image)};
        const auto end = std::chrono::steady_clock::now();
        std::cout << "time difference for sequential\t\t" << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms" << std::endl;
        const auto rescaled{rescaleOutput(output)};
        cv::imwrite("/Users/dominikaulinger/Desktop/sequential.jpg", rescaled);
    }

    {
        const auto begin = std::chrono::steady_clock::now();
        const auto output{computeIIParallel(image, 200)};
        const auto end = std::chrono::steady_clock::now();
        std::cout << "time difference for parallel\t\t" << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms" << std::endl;
        const auto rescaled{rescaleOutput(output)};
        cv::imwrite("/Users/dominikaulinger/Desktop/parallel.jpg", rescaled);
    }
}

int main() {
    std::cout << "running timer" << std::endl;
    realExample();
}