/*
直接用clang++运行
cd opencv_code/2.Opencv初探

clang++ -Wall -v -std=c++11 2_5_simpleVideoPlayer.cpp \
-I/usr/local/include/opencv4 `pkg-config --libs opencv4` -lncurses \
-o ../Execution/2_5_simpleVideoPlayer

或者直接指明要链接的库
clang++ -Wall -v -g -std=c++11 2_5_simpleVideoPlayer.cpp \
-I/usr/local/include/opencv4 \
-L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_videoio -lncurses \
-o ../Execution/2_5_simpleVideoPlayer

../Execution/2_5_simpleVideoPlayer ../Execution/demo.mp4
*/

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <ncurses.h>

#define KEY_BLANK 32
cv::VideoCapture g_cap;
int g_slider_position = 0;
int g_run = 1;
int g_dontset = 0;

static void onTrackbar(int pos, void *)
{
    g_cap.set(cv::CAP_PROP_POS_FRAMES, pos);
    if (!g_dontset)
        g_run = 1;
    g_dontset = 0;
}

int main(int argc, char **argv)
{
    initscr();            /* curses initialization */
    keypad(stdscr, TRUE); /* enable KEY_UP/KEY_DOWN/KEY_RIGHT/KEY_LEFT */
    noecho();

    cv::namedWindow("SimpleVideoPlayer", cv::WINDOW_AUTOSIZE);
    g_cap.open(argv[1]);
    int frames = (int)g_cap.get(cv::CAP_PROP_FRAME_COUNT);
    std::cout << "current video has " << frames << " frames" << std::endl;
    cv::createTrackbar("frames", "SimpleVideoPlayer", nullptr, frames, onTrackbar);
    cv::setTrackbarPos("frames", "SimpleVideoPlayer", g_slider_position);

    cv::Mat frame;
    for (;;)
    {
        if (g_run != 0)
        {
            g_cap >> frame;
            if (frame.empty())
                break;
            int current_pos = (int)g_cap.get(cv::CAP_PROP_POS_FRAMES);
            cv::setTrackbarPos("frames", "SimpleVideoPlayer", current_pos);
            cv::imshow("SimpleVideoPlayer", frame);
            g_run -= 1;
        }
        char c = cv::waitKey(10);
        if (c == KEY_BLANK)
        {
            if (g_run > 0)
                g_run = 0;
            if (g_run == 0)
                g_run = -1;
        }
        if (c == KEY_LEFT)
        {
        }
        if (c == KEY_RIGHT)
        {
        }
    }

    return 0;
}