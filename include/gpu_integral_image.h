#ifndef ZEISS_TASK_GPU_INTEGRAL_IMAGE_H
#define ZEISS_TASK_GPU_INTEGRAL_IMAGE_H

#include <opencv2/opencv.hpp>

cv::Mat computeIIParallelGPU(const cv::Mat& input, int gridSize = 3);

#endif //ZEISS_TASK_GPU_INTEGRAL_IMAGE_H