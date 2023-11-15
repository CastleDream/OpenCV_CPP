// 运行方式
// cd <DisplayImage_directory>
// cmake .
// cmake -D CMAKE_PREFIX_PATH=/Users/huangshan/Documents/DailyStudy/cpp/lib/build_opencv .
// make
// 然后就可以指定一个图像路径去进行显示了 ./DisplayImage ../lib/opencv-4.8.0/samples/data/lena.jpg

// /Users/huangshan/Documents/DailyStudy/cpp/opencv_code/build/DisplayImage
// /Users/huangshan/Documents/DailyStudy/cpp/lib/opencv-4.8.0/samples/data/lena.jpg

// 运行示例程序
// /Users/huangshan/Documents/DailyStudy/cpp/lib/build_opencv/bin/example_cpp_camshiftdemo
// /Users/huangshan/Documents/DailyStudy/cpp/lib/build_opencv/bin/example_cpp_kmeans

// 直接用clang++运行
// clang++ -Wall -v -std=c++11 DisplayImage.cpp -I/usr/local/include/opencv4 `pkg-config --libs opencv4` -o DisplayImage

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

    std::cout << "Image.size is " << image.size() << std::endl;

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
