/*
直接用clang++运行
cd opencv_code/2.Opencv初探

clang++ -Wall -v -std=c++11 temp.cpp \
-I/usr/local/include/opencv4 `pkg-config --libs opencv4` \
-o ../Execution/temp

../Execution/temp
*/

#include <iostream>
#include <opencv2/opencv.hpp>
int main()
{
    std::cout << cv::getBuildInformation() << std::endl;
    // Returns full configuration time cmake output
    return 0;
}