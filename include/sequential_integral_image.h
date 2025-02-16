#ifndef ZEISS_TASK_SEQUENTIAL_INTEGRAL_IMAGE_H
#define ZEISS_TASK_SEQUENTIAL_INTEGRAL_IMAGE_H

#include <opencv2/opencv.hpp>

#include "integral_image_constants.h"

cv::Mat computeIISequential(const cv::Mat& input);

#endif //ZEISS_TASK_SEQUENTIAL_INTEGRAL_IMAGE_H