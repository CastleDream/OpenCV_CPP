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
    // std::cout << cv::getBuildInformation() << std::endl;
    // Returns full configuration time cmake output

    // https://answers.opencv.org/question/221876/cvwaitkey-not-reading-keyboard-input-properly/

    /*
    关于waitKey函数和waitKeyEx()函数，是和GUI事件相关的，因此必须有处于运行状态的HighGUI窗口，这个函数才会工作
    @note The functions #waitKey and #pollKey are the only methods in HighGUI that can fetch and handle
    GUI events, so one of them needs to be called periodically for normal event processing unless
    HighGUI is used within an environment that takes care of event processing.

    📝📝📝📝📝
    @note The function only works if there is at least one HighGUI window created and the window is
    active. If there are several HighGUI windows, any of them can be active.
    */
    cv::Mat frame(2, 2, CV_8UC3, cv::Scalar(0, 0, 255));
    cv::namedWindow("test");
    cv::imshow("test", frame);
    while (1)
    {
        int key = cv::waitKeyEx(1);
        // 另外，想要获取waitKey期间的按下的值，必须选中窗口/焦点在窗口上，而不是在命令行里。。
        if (key > 0)
            std::cout << "key num is " << key << std::endl;
    }
    return 0;
}