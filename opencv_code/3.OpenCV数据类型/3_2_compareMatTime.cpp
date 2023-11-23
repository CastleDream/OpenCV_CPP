/*
切换到相应目录
cd opencv_code/3.OpenCV数据类型

clang++ -Wall -v -std=c++11 3_2_compareMatTime.cpp \
-I/usr/local/include/opencv4 `pkg-config --libs opencv4` \
-o ../Execution/3_2_compareMatTime

或者直接指明要链接的库
clang++ -Wall -v -std=c++11 3_2_compareMatTime.cpp \
-I/usr/local/include/opencv4 \
-L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_videoio \
-o ../Execution/3_2_compareMatTime

运行可执行文件
../Execution/3_2_compareMatTime
*/

#include <iostream>
#include <opencv2/opencv.hpp>
/*
测试一下矩阵运算C++和python的速度。
*/
int main(int argc, char **argv)
{
    cv::Mat time(512, 1024, CV_32F);
    cv::Mat time2(512, 1024, CV_32F);
    cv::randu(time, 1, 10);
    cv::randu(time2, 1, 10);

    time2 = time2.t();
    std::cout << "time height/rows is " << time.rows << " , width/cols is " << time.cols << std::endl;
    std::cout << "After t transposed, time2.t height/rows is " << time2.rows << " , width/cols is " << time2.cols << std::endl;

    double t = (double)cv::getTickCount();
    for (int i = 0; i < 1000; i++)
    {
        cv::Mat rs = time * time2;
    }
    // cv::Mat rs = time * time2;
    t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
    std::cout << "512*1024 matrix multiply time is: " << t << std::endl;
    return 0;
}