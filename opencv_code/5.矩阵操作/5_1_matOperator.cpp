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

// 查看bitwise函数/~的取反原理
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

// 查看函数calcCovarMatrix()的用法
void check_calcCovarMatrix()
{
}

/*笛卡尔坐标系转为极坐标系
https://stackoverflow.com/questions/57179932/precision-lost-calling-cv2-carttopolar-with-angleindegrees-set-to-false
https://stackoverflow.com/questions/26657336/putting-a-vector-of-points-through-carttopolar-in-opencv
https://docs.opencv.org/4.x/d3/d63/classcv_1_1Mat.html
*/
void check_cartToPolar()
{
    std::vector<cv::Point2f> points;       // 要计算弧度/角度，需要浮点数
    points.push_back(cv::Point2f(1, 1));   // 45°
    points.push_back(cv::Point2f(4, 0));   // 0°/360°
    points.push_back(cv::Point2f(0, 10));  // 90°
    points.push_back(cv::Point2f(0, -10)); // -270°

    // cv::Mat的构造函数
    // 	Mat (int ndims, const int *sizes, int type, const Scalar &s)
    // Mat(int ndims, const int *sizes, int type, void *data, const size_t *steps = 0)
    // cv::Mat有一个指针和step的构造函数，这里取x分量的头指针，然后每个2个float数字取一个数，(x,y,x,y)
    cv::Mat xpts(points.size(), 1, CV_32F, &points[0].x, 2 * sizeof(float));
    cv::Mat ypts(points.size(), 1, CV_32F, &points[0].y, 2 * sizeof(float));

    // 转置
    std::cout << "x: " << xpts << std::endl;
    // x:[1;
    //    4;
    //    0;
    //    0]
    std::cout << "x.t: " << xpts.t() << std::endl; // 转置  [1, 4, 0, 0]
    std::cout << "y.t: " << ypts.t() << std::endl; // [1, 0, 10, -10]

    // 转换
    cv::Mat magnitude, angle;
    cv::cartToPolar(xpts, ypts, magnitude, angle);
    // 可以直接设置angleInDegrees=True,就会直接返回角度而不是弧度了，也不用自己转换了
    // 而且这个可以返回0~360度的
    // 比atan(x,y)多的地方就是还返回了向量的模
    std::cout << "\nmagnitude: " << magnitude.t();                     // [1.4142135, 4, 10, 10]
    std::cout << "\nangle: " << angle.t() * 180. / CV_PI << std::endl; // [44.990456, 0, 90, 270]
}

int main(int argc, char **argv)
{
    // check_bitwise();
    check_cartToPolar();
    return 0;
}