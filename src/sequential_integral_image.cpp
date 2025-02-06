#include "sequential_integral_image.h"

cv::Mat computeIISequential(const cv::Mat& input) {
    cv::Mat output(input.rows, input.cols, II_CONST_CV_TYPE);
    std::vector<unsigned int> cumulativeRowSums(output.cols);
    for(int r{0}; r < output.rows; ++r) {
        unsigned int lastIIValue{0};
        for(int c{0}; c < output.cols; ++c) {
            cumulativeRowSums.at(c) += input.at<II_CONST_COMP_TYPE>(r, c);
            lastIIValue = cumulativeRowSums.at(c) + lastIIValue;
            output.at<II_CONST_COMP_TYPE>(r, c) = lastIIValue;
        }
    }
    return output;
}