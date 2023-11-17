/*
直接用clang++运行
cd opencv_code/2.Opencv初探

clang++ -Wall -v -std=c++11 2_5_simpleVideoPlayer.cpp \
-I/usr/local/include/opencv4 `pkg-config --libs opencv4` -lncurses \
-o ../Execution/2_5_simpleVideoPlayer

或者直接指明要链接的库
clang++ -Wall -v -std=c++11 2_5_simpleVideoPlayer.cpp \
-I/usr/local/include/opencv4 \
-L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_videoio -lncurses \
-o ../Execution/2_5_simpleVideoPlayer

../Execution/2_5_simpleVideoPlayer ../Execution/demo.mp4
*/

/*
关于调试，由于使用了ncurses库，调试过程中会接受外部键盘输入（不是argv传递的参数），所以一般调试时会需要另一个线程的加入。
参考：
https://stackoverflow.com/questions/13195203/ncurses-basic-example-in-debug-i-get-error-opening-terminal-unknown
https://stackoverflow.com/questions/11965328/debugging-ncurses-application-with-gdb
https://stackoverflow.com/questions/3938291/debugging-ncurses-with-eclipse-cdt

*/

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>

#define KEY_BLANK 32
#define KEY_LEFT 63234
#define KEY_RIGHT 63235

cv::VideoCapture g_cap;
int g_slider_position = 0;
int g_run = 1;
int g_dontset = 0; // g_dontset=0 说明是直接拖动滑动条，没有走if (g_run != 0)里，
// g_dontset=1 说明是走的f (g_run != 0)里

static void onTrackbar(int pos, void *)
{
    g_cap.set(cv::CAP_PROP_POS_FRAMES, pos);
    if (!g_dontset)
        g_run = 1;
    g_dontset = 0;
}

int main(int argc, char **argv)
{
    cv::namedWindow("SimpleVideoPlayer", cv::WINDOW_AUTOSIZE);
    g_cap.open(argv[1]);
    int frames = (int)g_cap.get(cv::CAP_PROP_FRAME_COUNT);
    std::cout << "current video has " << frames << " frames" << std::endl;
    cv::createTrackbar("frames", "SimpleVideoPlayer", nullptr, frames, onTrackbar);
    cv::setTrackbarPos("frames", "SimpleVideoPlayer", g_slider_position);

    cv::Mat frame;

    for (;;)
    {
        int current_pos;
        if (g_run != 0)
        {
            g_cap >> frame;
            if (frame.empty())
                break;
            g_dontset = 1;
            current_pos = (int)g_cap.get(cv::CAP_PROP_POS_FRAMES);
            cv::setTrackbarPos("frames", "SimpleVideoPlayer", current_pos);
            cv::imshow("SimpleVideoPlayer", frame);
            g_run -= 1;
        }
        int c = (int)cv::waitKeyEx(10);
        if (c == KEY_BLANK)
        {
            if (g_run < 0)
            {
                g_run = 1;
                // std::cout << "KEY_BLANK g_run = 0" << std::endl;
            }
            if (g_run == 0)
            {
                g_run = -1;
                // std::cout << "KEY_BLANK g_run = -1" << std::endl;
            }
        }
        if (c == KEY_LEFT && current_pos > 0)
        {
            cv::setTrackbarPos("frames", "SimpleVideoPlayer", current_pos - 2);
            cv::imshow("SimpleVideoPlayer", frame);
            std::cout << "KEY_LEFT" << std::endl;
        }
        if (c == KEY_RIGHT && current_pos < frames)
        {
            cv::setTrackbarPos("frames", "SimpleVideoPlayer", current_pos);
            cv::imshow("SimpleVideoPlayer", frame);
            std::cout << "KEY_RIGHT" << std::endl;
        }
        if (c == 27) // ESC键
            break;
    }

    return 0;
}
/*
关于这里按左键，位置-2，按右键，位置不加1的理由
1. 实验观察得到的，如果左键位置-1，则按下左键没有变换；右键+1，则按下右键的时候每次跳两帧。
*/