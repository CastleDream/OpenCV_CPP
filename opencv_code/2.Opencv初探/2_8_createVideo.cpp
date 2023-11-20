/*
@File    :   2_8_createVideo.cpp
@Time    :   2023/11/18 22:52:55
@Author  :   xxx
@Version :   1.0
@Contact :   XXX@outlook.com
@License :   (C)Copyright 2023~
@Desc    :   None

切换到相应目录
cd opencv_code/2.Opencv初探

clang++ -Wall -v -std=c++11 2_8_createVideo.cpp \
-I/usr/local/include/opencv4 `pkg-config --libs opencv4` -lncurses \
-o ../Execution/2_8_createVideo

或者直接指明要链接的库
clang++ -Wall -v -std=c++11 2_8_createVideo.cpp \
-I/usr/local/include/opencv4 \
-L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_videoio -lncurses \
-o ../Execution/2_8_createVideo

运行可执行文件
../Execution/2_8_createVideo ../Execution/demo.mp4 ../Execution/rs.mp4 1
1. 生成视频文件
2. 读取显示两个视频文件
3. 处理、显示、写一个视频文件
*/

#include <iostream>
#include <opencv2/opencv.hpp>
#include <unistd.h>

void help(char **argv)
{
    std::cout << "2_8_createVideo.cpp \n"
              << argv[1] << "argv[1]: video path, for example: ../Execution/demo.mp4 \n"
              << argv[2] << "argv[2]: video save path, for example: ../Execution/rs.mp4" << std::endl
              << argv[3] << "argv[3]: 1 - Gaussian,2 - Down Sample" << std::endl;
}

void showLogPolar(cv::VideoWriter &vw, cv::VideoCapture &vc, double fps)
{
    cv::Mat log_polar_frame, bgr_frame;
    int e1 = 0, e2 = 1;

    while (1)
    {
        vc >> bgr_frame;
        if (bgr_frame.empty())
            break;
        // log_polar_frame = cv::logPolar(bgr_frame);
        // This function produces same result as cv::warpPolar(src, dst, src.size(), center, maxRadius, flags+WARP_POLAR_LOG)
        cv::warpPolar(bgr_frame, log_polar_frame, bgr_frame.size(),
                      cv::Point2f(bgr_frame.cols / 2, bgr_frame.rows / 2), 40, cv::WARP_FILL_OUTLIERS);
        // fills all of the destination image pixels. If some of them correspond to outliers in the
        // source image, they are set to zero  WARP_FILL_OUTLIERS = 8,
        vw << log_polar_frame;
    }
    vw.release();
    e2 = cv::getTickCount();
    double wait_time = (e2 - e1) / cv::getTickFrequency();
    // the number of ticks per second
    std::cout << "wait for writing: " << wait_time << std::endl;
}

/*
只显示某个BGR视频的某个通道。自：https://docs.opencv.org/4.x/d7/d9e/tutorial_video_write.html
*/
void showRGB(cv::VideoWriter &vw, cv::VideoCapture &vc)
{
    int channel = 2; // 只显示r通道，读取的图像是bgr的顺序
    cv::namedWindow("source_video", cv::WINDOW_NORMAL);
    cv::Size S = cv::Size((int)vc.get(cv::CAP_PROP_FRAME_WIDTH), // Acquire input size
                          (int)vc.get(cv::CAP_PROP_FRAME_HEIGHT));
    cv::Mat src, res;
    std::vector<cv::Mat> spl;

    if (!vc.isOpened())
    {
        std::cout << "Could not open the input video: " << std::endl;
    }
    if (!vw.isOpened())
    {
        std::cout << "Could not open the output video for write:" << std::endl;
    }
    for (;;)
    {
        vc >> src;
        if (src.empty())
            break;           // check if at end
        cv::split(src, spl); // process - extract only the correct channel
        for (int i = 0; i < 3; ++i)
            if (i != channel)
                spl[i] = cv::Mat::zeros(S, spl[0].type());
        cv::merge(spl, res);
        cv::imshow("source_video", res);
        int c = cv::waitKey(1);
        if (c == 113) // 27,esc键  113,q键
        {
            std::cout << "key  pressed" << std::endl;
            break;
        }
        vw << res;
    }
    std::cout << "Finished writing" << std::endl;
    cv::destroyAllWindows();
    vw.release();
    vc.release();
}

void showTwoVideo(cv::VideoCapture &vc, cv::VideoCapture &rs, double fps)
{
    cv::Mat log_polar_frame, bgr_frame;
    cv::namedWindow("source_video", cv::WINDOW_NORMAL);
    cv::namedWindow("rs_video", cv::WINDOW_NORMAL);
    // WINDOW_AUTOSIZE→WINDOW_NORMAL
    while (1)
    {
        vc >> bgr_frame;
        rs >> log_polar_frame;
        cv::imshow("source_video", bgr_frame);
        cv::imshow("rs_video", log_polar_frame);

        // Waits for a pressed key, Delay in milliseconds(毫秒)
        // fps: frames per second,如果fps是30，则就是1s30帧，那么delay时间是： 1/30*1000
        int c = cv::waitKey(1000 / fps);
        // 如果是显示一个图像，cv::waitKey(0);相当于一直sleep，直到有任意键按下
        // 如果是显示一个视频，那么这里的waitKey的时间应该是和帧数时间是一致的，这样显示图像切换时间就和帧播放时间一致了
        // 例如：cv::waitKey(33);就是指当前这个窗口的帧显示33ms，然后就去执行下面的语句。

        if (c == 113) // 27,esc键  113,q键
        {
            std::cout << "key  pressed" << std::endl;
            break;
        }
    }
    cv::destroyAllWindows();
}

int main(int argc, char **argv)
{

    if (argc != 4)
    {
        help(argv);
        return 0;
    }
    cv::VideoCapture src_cap(argv[1]);
    // src_cap.open(argv[1]);
    double fps = src_cap.get(cv::CAP_PROP_FPS);
    cv::Size size((int)src_cap.get(cv::CAP_PROP_FRAME_WIDTH), (int)src_cap.get(cv::CAP_PROP_FRAME_HEIGHT));

    std::cout << "fps is: " << fps << ", size is: width: " << size.width << ", height: " << size.height << std::endl;

    // int ex = static_cast<int>(src_cap.get(cv::CAP_PROP_FOURCC));
    cv::VideoWriter rs_write(argv[2], cv::VideoWriter::fourcc('m', 'p', '4', 'v'), fps, size);
    // cv::VideoWriter rs_write(argv[2], cv::VideoWriter::fourcc('a', 'v', 'c', '1'), fps, size);
    // cv::VideoWriter rs_write(argv[2], cv::CAP_FFMPEG, cv::VideoWriter::fourcc('a', 'v', 'c', '1'), fps, size);

    char program = *argv[3];
    if (program == '1')
    {
        showLogPolar(rs_write, src_cap, fps);
        rs_write.release();
        src_cap.release();
        // 报错：error: (-215:Assertion failed) size.width>0 && size.height>0 in function 'imshow'
        // 如果一个VideoCapture已经for循环到最后一帧，此时再用它去读原始的视频，就会报上面的错误，即便isOpened()返回true
        // 但是停留在最后一帧的后面，因此读不到帧，导致size.width>0 && size.height>0判断错误。
    }
    if (program == '2')
    {
        showRGB(rs_write, src_cap);
        rs_write.release();
        src_cap.release();
    }
    if (program == '3')
    {
        cv::VideoCapture src_cap(argv[1]);
        // cv::VideoCapture rs_cap(argv[2]);
        cv::VideoCapture rs_cap("../Execution/rs的副本.mp4");
        if (rs_cap.isOpened())
        {
            std::cout << "isOpened true" << std::endl;
            showTwoVideo(src_cap, rs_cap, fps);
        }
    }

    return 0;
}