#include <iostream>

#include "integral_image.h"

int main() {
    std::cout << "running timer" << std::endl;
    const auto example{integralImageInputExampleLargeRandom()};
    {
        const auto begin = std::chrono::steady_clock::now();
        const auto output{computeIISequential(example)};
        const auto end = std::chrono::steady_clock::now();
        std::cout << "time difference for sequential\t\t" << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms" << std::endl;
    }

    {
        const auto begin = std::chrono::steady_clock::now();
        const auto output{computeIIParallel(example, 100)};
        const auto end = std::chrono::steady_clock::now();
        std::cout << "time difference for parallel\t\t" << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms" << std::endl;
    }
}