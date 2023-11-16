/*
直接用clang++运行
cd opencv_code/2.Opencv初探

clang++ -Wall -v -std=c++11 2_3_readVideo.cpp \
-I/usr/local/include/opencv4 `pkg-config --libs opencv4` \
-o ../Execution/2_3_readVideo

或者直接指明要链接的库
clang++ -Wall -v -std=c++11 2_3_readVideo.cpp \
-I/usr/local/include/opencv4 \
-L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_videoio \
-o ../Execution/2_3_readVideo

../Execution/2_3_readVideo ../Execution/demo.mp4
*/

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>

int main(int argc, char **argv)
{
    cv::namedWindow("Example2");
    cv::VideoCapture cap;
    // VideoCapture可以打开和关闭很多ffmpeg支持的视频文件
    cap.open(std::string(argv[1]));
    cv::Mat frame;
    if (cap.isOpened())
    {
        for (;;)
        {
            cap >> frame;
            // Stream operator to read the next video frame
            if (frame.empty())
                break;
            cv::imshow("Example2", frame);
            if (cv::waitKey(33) >= 0)
                // 33ms→ 30FPS播放，高于人眼的24FPS，其实更好的做法是读取视频的帧率信息去设置这个值
                break;
        }
    }
    return 0;
}