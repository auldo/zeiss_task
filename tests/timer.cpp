#include <iostream>

#include "integral_image.h"

int main() {
    std::cout << "running timer" << std::endl;
    const auto example{integralImageInputExampleWuEtAl()};

    {
        const auto begin = std::chrono::steady_clock::now();
        const auto end = std::chrono::steady_clock::now();
        const auto output{computeIISequential(example)};
        std::cout << "time difference for sequential\t\t" << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() << " ns" << std::endl;

    }

    {
        const auto begin = std::chrono::steady_clock::now();
        const auto end = std::chrono::steady_clock::now();
        computeIIParallel(example);
        std::cout << "time difference for parallel\t\t" << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() << " ns" << std::endl;

    }
}