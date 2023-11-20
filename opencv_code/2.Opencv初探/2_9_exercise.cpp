/*
切换到相应目录
cd opencv_code/2.Opencv初探

clang++ -Wall -v -std=c++11 2_9_exercise.cpp \
-I/usr/local/include/opencv4 `pkg-config --libs opencv4` \
-o ../Execution/2_9_exercise

或者直接指明要链接的库
clang++ -Wall -v -std=c++11 2_9_exercise.cpp \
-I/usr/local/include/opencv4 \
-L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_videoio \
-o ../Execution/2_9_exercise

运行可执行文件
../Execution/2_9_exercise ../Execution/lena.jpg

运行OpenCV附带的示例：
因为安装的时候就编译了，所以可以直接跑，例如：
../lib/build_opencv/bin/example_cpp_lkdemo

也可以找到相应的cpp去用clang++运行
clang++ -Wall -v -std=c++11 ~/Documents/DailyStudy/cpp/lib/opencv-4.8.0/samples/cpp/lkdemo.cpp \
-I/usr/local/include/opencv4 `pkg-config --libs opencv4` \
-o ../Execution/example_cpp_lkdemo

../Execution/example_cpp_lkdemo
*/

#include <iostream>
#include <opencv2/opencv.hpp>

cv::Mat image;
cv::Mat downImage;

void on_Track(int downNum, void *)
{ // 为了保证下采样是偶数倍，因此2~8变成1~4，实际处理的时候乘以2即可
    // https://stackoverflow.com/questions/54055124/odd-numbers-only-on-opencv-trackbar-for-python
    // downNum *= 2;

    // cv::pyrDown(downImage, downImage, size);
    // 根据https://answers.opencv.org/question/209979/assertion-failed-pyrup-pyrdown/
    // error: (-215:Assertion failed) ssize.width > 0 && ssize.height > 0 && std::abs(dsize.width*2 - ssize.width) <= 2 && std::abs(dsize.height*2 - ssize.height) <= 2 in function 'pyrDown_'
    // assert声明已经足够说明，这个pyrDown函数只支持下采样因子为2，不支持任意倍率的下采样
    // 如果要进行任意倍率的下采样，请使用resize
    cv::Size size(cv::Size((image.cols + 1) / downNum, (image.rows + 1) / downNum));
    cv::resize(image, downImage, size);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "Please input an image path" << std::endl;
    }
    image = cv::imread(argv[1]);
    std::cout << "image width is: " << image.cols << ", height is: " << image.rows << std::endl;
    downImage = cv::imread(argv[1]);

    cv::namedWindow("pyramid", cv::WINDOW_AUTOSIZE);

    cv::createTrackbar("downSample", "pyramid", nullptr, 8, on_Track);
    cv::setTrackbarMax("downSample", "pyramid", 8);
    cv::setTrackbarMin("downSample", "pyramid", 1);
    cv::setTrackbarPos("downSample", "pyramid", 1);

    while (1)
    {
        cv::imshow("pyramid", downImage);
        int c = cv::waitKey(1);
        if (c == 27)
            break;
    }
    cv::destroyAllWindows();
    return 0;
}