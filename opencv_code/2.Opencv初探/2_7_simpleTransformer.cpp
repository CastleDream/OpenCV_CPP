
/*
@File    :   2_7_simpleTransformer.cpp
@Time    :   2023/11/18 22:04:08
@Author  :   XXXX
@Version :   1.0
@Contact :   XXX@outlook.com
@License :   (C)Copyright 2023~
@Desc    :   None
*/
/*直接用clang++运行
cd opencv_code/2.Opencv初探

clang++ -Wall -v -std=c++11 2_7_simpleTransformer.cpp \
-I/usr/local/include/opencv4 `pkg-config --libs opencv4` -lncurses \
-o ../Execution/2_7_simpleTransformer

或者直接指明要链接的库

clang++ -Wall -v -std=c++11 2_7_simpleTransformer.cpp
-I/usr/local/include/opencv4
-L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_videoio -lncurses
-o ../Execution/2_7_simpleTransformer

../Execution/2_7_simpleTransformer ../../lib/opencv-4.8.0/samples/data/lena.jpg 1
*/

#include <iostream>
#include <opencv2/opencv.hpp>

void help(char **argv)
{
    std::cout << "2_7_simpleTransformer.cpp \n "
              << argv[1] << "argv[1]: image path, for example: ..lib/opencv-4.8.0/samples/data/lena.jpg " << std::endl
              << argv[2] << "argv[2]: 1-Gaussian, 2-Down Sample, 3-Canny, 4-Access Pixel Value" << std::endl;
}

void gaussianBlur(const cv::Mat &image)
{
    cv::Mat out;
    cv::namedWindow("image out", cv::WINDOW_AUTOSIZE);
    cv::GaussianBlur(image, out, cv::Size(5, 5), 3, 3);
    cv::GaussianBlur(out, out, cv::Size(5, 5), 3, 3);
    // sigmaY – Gaussian kernel standard deviation in Y direction
    // 显示高斯模糊后的图像
    cv::imshow("image out", out);
    cv::waitKey(0);
}

void downSample(const cv::Mat &image)
{
    cv::Mat out;
    cv::namedWindow("image out", cv::WINDOW_AUTOSIZE);
    cv::pyrDown(image, out);
    // Blurs an image and downsamples it.
    // By default, size of the output image is computed as Size((src.cols+1)/2, (src.rows+1)/2)
    cv::imshow("image out", out);
    cv::waitKey(0);
}

void cannyDetect(const cv::Mat &image)
{
    cv::Mat out;
    cv::namedWindow("image out", cv::WINDOW_AUTOSIZE);
    cv::cvtColor(image, out, 6);
    // COLOR_BGR2GRAY     = 6,
    cv::Canny(out, out, 10, 100, 3, true);
    cv::imshow("image out", out);
    cv::waitKey(0);
}

void accessPixel(const cv::Mat &image)
{
    cv::Mat out;
    cv::cvtColor(image, out, cv::COLOR_BGR2GRAY);
    cv::pyrDown(out, out);
    cv::pyrDown(out, out);
    cv::Canny(out, out, 10, 100, 3, true);
    int x = 16, y = 32;
    // typedef Vec<uchar, 3> Vec3b;
    // out.at<cv::Vec3b> 模版函数，传入模版类型是cv::Vec3b
    cv::Vec3b intensity = image.at<cv::Vec3b>(y, x);
    uint8_t blue = intensity[0];
    uint8_t green = intensity[1];
    uint8_t red = intensity[2];

    std::cout << "At (x,y) = (" << x << " , " << y
              << " ): (blue, green, red) = ("
              << (uint32_t)blue << " , " << (uint32_t)green << " , " << (uint32_t)red << ")" << std::endl;
    std::cout << "At (x,y) = (" << x << " , " << y
              << " ): (gray) = (" << (uint32_t)out.at<uint8_t>(y, x) << ")" << std::endl;
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        help(argv);
        return 0;
    }
    cv::Mat image = cv::imread(argv[1]);
    cv::namedWindow("image in", cv::WINDOW_AUTOSIZE);
    cv::imshow("image in", image);

    char program = *argv[2];
    if (program == '1')
        gaussianBlur(image);
    if (program == '2')
        downSample(image);
    if (program == '3')
        cannyDetect(image);
    if (program == '4')
        accessPixel(image);
    return 0;
}