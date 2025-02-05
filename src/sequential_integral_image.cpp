#include "sequential_integral_image.h"

cv::Mat computeIISequential(const cv::Mat& input) {
    cv::Mat output(input.rows, input.cols, CV_16UC1);
    std::vector<unsigned int> cumulativeRowSums(output.cols);
    for(int r{0}; r < output.rows; ++r) {
        unsigned int lastIIValue{0};
        for(int c{0}; c < output.cols; ++c) {
            cumulativeRowSums.at(c) += input.at<uint16_t>(r, c);
            lastIIValue = cumulativeRowSums.at(c) + lastIIValue;
            output.at<uint16_t>(r, c) = lastIIValue;
        }
    }
    return output;
}