/*
直接用clang++运行
cd opencv_code/2.Opencv初探

clang++ -Wall -v -std=c++11 2_2_showVideo.cpp \
-I/usr/local/include/opencv4 `pkg-config --libs opencv4` \
-o ../Execution/2_2_openCamera

或者直接指明要链接的库
clang++ -Wall -v -std=c++11 2_2_openCamera.cpp \
-I/usr/local/include/opencv4 \
-L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_videoio \
-o ../Execution/2_2_openCamera

../Execution/2_2_openCamera
*/

#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>

using namespace cv;
int main(int argc, char **argv)
{
    Mat frame;
    // 初始化一个VIDEOCAPTURE对象cap
    VideoCapture cap;
    // 使用默认的API打开默认的相机
    // cap.open(0);
    // 或者更先进的用法，选择任意一个API后端
    int deviceID = 0;        // 0 = open default camera
    int apiID = cv::CAP_ANY; // 0 = autodetect default API
    // 使用选择的API打开选择的相机
    cap.open(deviceID, apiID);
    // 判断是否正确打开视频流
    if (!cap.isOpened())
    {
        std::cerr << "ERROR! Unable to open camera\n";
        return -1;
    }
    // 开始读取视频
    std::cout << "Start grabbing" << std::endl
              << "Press any key to terminate" << std::endl;
    for (;;)
    {
        // 等待从视频读入新帧并把它存储到'frame'中
        cap.read(frame);
        // 检查是否读取成功
        if (frame.empty())
        {
            std::cerr << "ERROR! blank frame grabbed\n";
            break;
        }
        // 实时显示，每5ms显示一帧图像直到有任意按键
        imshow("Live", frame);
        if (waitKey(5) >= 0)
            break;
    }
    // 相机对象会在VideoCapture析构函数中自动释放
    return 0;
}
