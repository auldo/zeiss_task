#ifndef ZEISS_TASK_PARALLEL_INTEGRAL_IMAGE_H
#define ZEISS_TASK_PARALLEL_INTEGRAL_IMAGE_H

#include <opencv2/opencv.hpp>
#include <thread>
#include <stack>
#include <vector>

cv::Mat computeIIParallel(const cv::Mat& input, int gridSize = 3);

#endif //ZEISS_TASK_PARALLEL_INTEGRAL_IMAGE_H