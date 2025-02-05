#ifndef ZEISS_TASK_PARALLEL_INTEGRAL_IMAGE_H
#define ZEISS_TASK_PARALLEL_INTEGRAL_IMAGE_H

#include <opencv2/opencv.hpp>

cv::Mat computeIIParallel(const cv::Mat& input);

#endif //ZEISS_TASK_PARALLEL_INTEGRAL_IMAGE_H