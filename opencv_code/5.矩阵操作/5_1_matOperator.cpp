/*
@File    :   5_1_matOperator.cpp
@Time    :   2024/01/29 22:02:13
@Author  :   xxx
@Version :   1.0
@Contact :   XXX@outlook.com
@License :   (C)Copyright 2023~
@Desc    :   None

切换到相应目录
cd ~/Documents/DailyStudy/cpp/opencv_code/5.矩阵操作

clang++ -Wall -v -std=c++11 5_1_matOperator.cpp \
-I/usr/local/include/opencv4 `pkg-config --libs opencv4` \
-o ../Execution/5_1_matOperator

运行可执行文件
../Execution/5_1_matOperator
*/

#include <iostream>
#include <opencv2/opencv.hpp>
void check_bitwise()
{
    cv::Mat_<cv::Vec<uchar, 2>> test(3, 3, cv::Vec2b(0, 255));
    cv::Mat a = ~test;
    std::cout << a << std::endl;
    /* a=[ 255, 0, 255, 0, 255, 0;
       255, 0, 255, 0, 255, 0;
       255, 0, 255, 0, 255, 0 ]

       test=[  0, 255,   0, 255,   0, 255;
        0, 255,   0, 255,   0, 255;
        0, 255,   0, 255,   0, 255]

        unsigned char是8位数字, 255的二进制表示为11111111（8个1） 取反变成00000000（8个0）
    */
    std::cout << test << std::endl;

    cv::Mat m(3, 3, CV_16SC1, cv::Scalar(1, 2));
    cv::Mat b = ~m;
    std::cout << "b is " << b << std::endl;
    std::cout << "m is " << m << std::endl;
    /*b is [-2, -2, -2;
        -2, -2, -2;
        -2, -2, -2]
      m is [1, 1, 1;
        1, 1, 1;
        1, 1, 1]
    */
    // cv::Mat c = !m;
    // std::cout << "c is " << b << std::endl;
}
int main(int argc, char **argv)
{
    check_bitwise();
    return 0;
}