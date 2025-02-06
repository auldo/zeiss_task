#ifndef ZEISS_TASK_INTEGRAL_IMAGE_INPUT_EXAMPLES_H
#define ZEISS_TASK_INTEGRAL_IMAGE_INPUT_EXAMPLES_H

#include <opencv2/opencv.hpp>
#include <random>

#include "integral_image_constants.h"

cv::Mat integralImageInputExampleWuEtAl();
cv::Mat integralImageInputExampleLargeRandom(int size = 2000, std::optional<int> fill = std::nullopt);

#endif //ZEISS_TASK_INTEGRAL_IMAGE_INPUT_EXAMPLES_H