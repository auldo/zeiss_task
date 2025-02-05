#include "integral_image_input_examples.h"

/// Returns the example from Wu et al. (2011)
cv::Mat integralImageInputExampleWuEtAl() {
    cv::Mat mat(9, 9, CV_16UC1);
    for(int i = 0; i < 3; ++i) {
        for(int r{i * 3}; r <= (i * 3 + 2); ++r) {
            for(int c{0}; c <= 2; ++c)
                mat.at<uint16_t>(r, c) = i * 3 + 1;
        }
        for(int r{i * 3}; r <= (i * 3 + 2); ++r) {
            for(int c{3}; c <= 5; ++c)
                mat.at<uint16_t>(r, c) = i * 3 + 2;
        }
        for(int r{i * 3}; r <= (i * 3 + 2); ++r) {
            for(int c{6}; c <= 8; ++c)
                mat.at<uint16_t>(r, c) = i * 3 + 3;
        }

    }
    return mat;
}

cv::Mat integralImageInputExampleLargeRandom() {
    std::random_device seeder;
    std::mt19937 engine(seeder());
    std::uniform_int_distribution<int> dist(1, 9);

    cv::Mat mat(2000, 2000, CV_16UC1);
    for(auto i{0}; i < mat.cols; ++i) {
        for(auto j{0}; j < mat.rows; ++j) {
            mat.at<uint16_t>(i, j) = dist(engine);
        }
    }
    return mat;
}