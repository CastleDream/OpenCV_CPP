/*
切换到相应目录
cd ~/Documents/DailyStudy/cpp/opencv_code/4.图像和大型数组类型

clang++ -Wall -v -std=c++11 4_5_chunkAcccessMat.cpp \
-I/usr/local/include/opencv4 `pkg-config --libs opencv4` \
-o ../Execution/4_5_chunkAcccessMat

运行可执行文件
../Execution/4_5_chunkAcccessMat
*/

#include <iostream>
#include <opencv2/opencv.hpp>

void compare_OPT()
{
    cv::Mat m1(3, 3, CV_8SC3, cv::Scalar(2.0, 4.3, 5.2));
    cv::Mat m2(3, 3, CV_8SC3, cv::Scalar(1.0, 3.3, 6.2));
    cv::Mat m3 = m1 > m2;
    std::cout << m3 << std::endl;
    /*
    [255, 255,   0, 255, 255,   0, 255, 255,   0;
    255, 255,   0, 255, 255,   0, 255, 255,   0;
    255, 255,   0, 255, 255,   0, 255, 255,   0]
    满足条件是255（true），不满足条件是0（false）
    */
}

void compare_at_block()
{

    cv::Mat image = cv::imread("../Execution/lena.jpg", cv::IMREAD_COLOR);
    std::cout << "cv::Mat的()重载" << std::endl;
    std::cout << "调用at的输出结果: " << image.rowRange(0, 2) << std::endl;
    std::cout << "operator()(Range rowRange,Range colRange) 输出结果: "
              << image(cv::Range(0, 2), cv::Range::all()) << std::endl;

    cv::Mat_<cv::Vec<u_char, 3>> image2;
    image2 = cv::imread("../Execution/lena.jpg", cv::IMREAD_COLOR);
    std::cout << "cv::Mat_<_Tp>的()重载" << std::endl;
    std::cout << "调用at的输出结果: " << image.at<cv::Vec3b>(10, 10) << std::endl;
    std::cout << "operator()(int row, int col) 输出结果: " << image2(10, 10) << std::endl;
}
int main(int argc, char **argv)
{
    // compare_OPT();
    compare_at_block();
    return 0;
}