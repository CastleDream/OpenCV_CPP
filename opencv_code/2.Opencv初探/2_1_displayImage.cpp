/* 直接用clang++运行
cd opencv_code/2.Opencv初探

clang++ -Wall -v -std=c++11 2_1_displayImage.cpp \
-I/usr/local/include/opencv4 `pkg-config --libs opencv4` \
-o ../Execution/2_1_displayImage

../Execution/2_1_displayImage （一个图片地址，例如：）/Users/huangshan/Documents/DailyStudy/cpp/lib/opencv-4.8.0/samples/data/lena.jpg

运行示例程序
/Users/huangshan/Documents/DailyStudy/cpp/lib/build_opencv/bin/example_cpp_camshiftdemo
/Users/huangshan/Documents/DailyStudy/cpp/lib/build_opencv/bin/example_cpp_kmeans

/Users/huangshan/Documents/DailyStudy/cpp/lib/build_opencv/bin/example_cpp_grabcut
或者先cd 再运行
cd /Users/huangshan/Documents/DailyStudy/cpp/lib/build_opencv/bin/
./example_cpp_grabcut

*/

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "usage: DisplayImage.out <Image_Path>\n";
        return -1;
    }
    Mat image;
    image = imread(argv[1], IMREAD_COLOR);

    if (!image.data)
    {
        std::cout << "No image data \n";
        return -1;
    }
    namedWindow("Display Image", WINDOW_AUTOSIZE); // HighGUI提供的函数
    imshow("Display Image", image);
    waitKey(0); // 等待键盘输入
    destroyWindow("Display Image");
    return 0;
}
