#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "integral_image.h"

cv::Mat rescaleOutput(const cv::Mat& output) {
    cv::Mat rescaled;
    cv::normalize(output, rescaled, 0, 255, cv::NORM_MINMAX, II_CONST_CV_TYPE);
    cv::Mat converted;
    rescaled.convertTo(converted, CV_8UC1);
    return converted;
}

void checkCorrectness(const cv::Mat& output) {
    //Check trivial output
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(0, 0), 1);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(0, 1), 2);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(0, 2), 3);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(0, 3), 5);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(0, 4), 7);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(0, 5), 9);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(0, 6), 12);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(0, 7), 15);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(0, 8), 18);

    //Check topmost output
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(1, 0), 2);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(1, 1), 4);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(1, 2), 6);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(1, 3), 10);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(1, 4), 14);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(1, 5), 18);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(1, 6), 24);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(1, 7), 30);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(1, 8), 36);

    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(2, 0), 3);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(2, 1), 6);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(2, 2), 9);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(2, 3), 15);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(2, 4), 21);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(2, 5), 27);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(2, 6), 36);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(2, 7), 45);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(2, 8), 54);

    //Check leftmost output
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(3, 0), 7);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(3, 1), 14);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(3, 2), 21);

    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(4, 0), 11);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(4, 1), 22);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(4, 2), 33);

    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(5, 0), 15);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(5, 1), 30);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(5, 2), 45);

    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(6, 0), 22);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(6, 1), 44);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(6, 2), 66);

    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(7, 0), 29);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(7, 1), 58);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(7, 2), 87);

    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(8, 0), 36);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(8, 1), 72);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(8, 2), 108);

    //Check remaining output
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(3, 3), 32);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(3, 4), 43);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(3, 5), 54);

    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(4, 3), 49);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(4, 4), 65);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(4, 5), 81);

    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(5, 3), 66);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(5, 4), 87);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(5, 5), 108);

    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(6, 3), 95);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(6, 4), 124);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(6, 5), 153);

    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(7, 3), 124);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(7, 4), 161);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(7, 5), 198);

    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(8, 3), 153);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(8, 4), 198);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(8, 5), 243);

    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(3, 6), 69);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(3, 7), 84);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(3, 8), 99);

    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(4, 6), 102);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(4, 7), 123);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(4, 8), 144);

    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(5, 6), 135);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(5, 7), 162);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(5, 8), 189);

    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(6, 6), 189);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(6, 7), 225);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(6, 8), 261);

    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(7, 6), 243);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(7, 7), 288);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(7, 8), 333);

    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(8, 6), 297);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(8, 7), 351);
    CHECK_EQ(output.at<II_CONST_COMP_TYPE>(8, 8), 405);
}

TEST_CASE("check wu et al. example sequential correctness") {
    const cv::Mat example{integralImageInputExampleWuEtAl()};
    const cv::Mat output{computeIISequential(example)};
    checkCorrectness(output);
    cv::imwrite("/Users/dominikaulinger/Desktop/small.png", rescaleOutput(output));
}

TEST_CASE("check wu et al. example parallel correctness") {
    std::cout << "Number of cores: " << std::thread::hardware_concurrency() << std::endl;
    const cv::Mat example{integralImageInputExampleWuEtAl()};
    for (int i{1}; i <= 9; ++i) {
        const cv::Mat output{computeIIParallel(example, i)};
        checkCorrectness(output);
    }
}