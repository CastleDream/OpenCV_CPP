/*
直接用clang++运行
cd opencv_code/2.Opencv初探

clang++ -Wall -v -std=c++11 2_4_trackBar.cpp \
-I/usr/local/include/opencv4 `pkg-config --libs opencv4` \
-o ../Execution/2_4_trackBar

或者直接指明要链接的库
clang++ -Wall -v -std=c++11 2_4_trackBar.cpp \
-I/usr/local/include/opencv4 \
-L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_videoio \
-o ../Execution/2_4_trackBar

../Execution/2_4_trackBar ../Execution/demo.mp4
*/

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>

/*
 添加滑动条，使得用户可以进行视频跳转
 允许用户通过S键来执行单步模式(g_run=0)；
 R键回复连续播放模式（g_run=1）
 */

// g_前缀表示全局变量
int g_slider_position = 0;
// 表示滑动条位置的全局变量
int g_run = 1;
// 产生跳转触发之后置为0，正值表示停止之前程序要播放多少张图片（默认1张），负值表示出于连续视频播放模式
// 运行一开始，显示并停在第一帧（for循环还一直在运行），只是因为g_run=0了，且此时g_dontset=0
// 拖动滑动条，执行一次on_trackbar，执行过程中，由于g_dontset=0,则g_run=1
// 可以执行一次if (g_run != 0)，此时获取到的滑动条的值是拖动后的值，不是s单步模式触发的值
int g_dontset = 0;
// 防止冲突，避免在调整进度条时触发单步模式
cv::VideoCapture g_cap;

// 回调函数：用于更改g_slider_position变量来重新定位视频读取的位置
static void on_trackbar(int pos, void *)
{
    g_cap.set(cv::CAP_PROP_POS_FRAMES, pos);
    // 查看 VideoCaptureProperties
    // 0-based index of the frame to be decoded/captured next
    if (!g_dontset)
        g_run = 1;
    g_dontset = 0;
}

int main(int argc, char **argv)
{
    cv::namedWindow("Example4", cv::WINDOW_GUI_EXPANDED);
    g_cap.open(argv[1]);

    // 获取读入视频的帧数和高宽（每帧的高宽都是一样的）
    int frames = (int)g_cap.get(cv::CAP_PROP_FRAME_COUNT); // Number of frames in the video file.
    int tmpW = (int)g_cap.get(cv::CAP_PROP_FRAME_WIDTH);   // Width of the frames in the video stream.
    int tmpH = (int)g_cap.get(cv::CAP_PROP_FRAME_HEIGHT);  // Height of the frames in the video stream.
    std::cout << "输入的视频有" << frames << "帧, (宽度, 高度)为"
              << "(" << tmpW << ", " << tmpH << ")" << std::endl;
    cv::createTrackbar("frame", "Example4", nullptr, frames, on_trackbar);
    cv::setTrackbarPos("frame", "Example4", g_slider_position);

    // const cv::String &trackbarname, 创建的滑动条的名字/显示
    // const cv::String &winname, 滑动条的父窗口的名字
    // int *value, 指向一个整数变量的指针，该整数变量的值会影响滑动条的位置。 一旦创建，滑动条的位置就由这个变量决定
    // int count,滑动条的最大位置，最小位置永远是0
    // cv::TrackbarCallback onChange = (cv::TrackbarCallback)0, 指向一个每次滑动条位置改变都会调用的函数，这个函数的原型应该类似于：void Foo(int,void*）
    // void *userdata = (void *)0

    cv::Mat frame;
    for (;;)
    {
        if (g_run != 0)
        {
            g_cap >> frame;
            if (frame.empty())
                break;

            // 设置滑动条的显示
            int current_pos = (int)g_cap.get(cv::CAP_PROP_POS_FRAMES);
            // 直接拖动滑动条而不是s键触发时，不进入单步模式
            g_dontset = 1;
            cv::setTrackbarPos("frame", "Example4", current_pos);
            // const cv::String &trackbarname, const cv::String &winname, int pos
            // 每次执行这个函数（滑动条位置改变），就会执行回调函数on_trackbar(视频显示帧改变)

            cv::imshow("Example4", frame);
            g_run -= 1;
            // 保持单步模式，或者保持run模式
            // 每显示一帧（10ms）就停下来判断一下有没有键盘输入。
        }
        char c = (char)cv::waitKey(10);
        if (c == 's')
        {
            g_run = 1;
            // g_run = 1, 程序只读取一张图片
            std::cout << "单步运行模式， run = " << g_run << std::endl;
        }
        if (c == 'r')
        {
            g_run = -1;
            // g_run=-1, 逐次递减持续运行
            std::cout << "连续播放模式， run = " << g_run << std::endl;
        }
        if (c == 27) // ESC键
            break;
    }
    return 0;
}