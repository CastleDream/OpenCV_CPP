#include <opencv2/opencv.hpp>
#include <iostream>
void showImg(cv::Mat image)
{
    cv::namedWindow("Show", cv::WINDOW_AUTOSIZE);
    cv::imshow("Show", image);
    if (113 == cv::waitKey(0))
    {
        cv::destroyWindow("Show");
    }
}

// 画圆+显示
void exercise_1()
{
    // cv::Mat aMat = cv::Mat(cv::Size(100, 100), CV_8UC3);
    cv::Mat aMat = cv::Mat::zeros(cv::Size(100, 100), CV_8UC3);
    cv::circle(aMat, cv::Point(50, 50), 20, cv::Scalar::all(255));
    showImg(aMat);
}

// 用cv::Mat访问元素方式画一个绿色方框
void exercise_2()
{
    cv::Mat aMat = cv::Mat::zeros(cv::Size(100, 100), CV_8UC3);
    aMat(cv::Range(20, 40), cv::Range(5, 20)) = cv::Vec3b(0, 255, 0);
    showImg(aMat);
}

// 用ptr指针访问元素方式画一个绿色方框
void exercise_3()
{
    cv::Mat aMat = cv::Mat::zeros(cv::Size(100, 100), CV_8UC3);
    for (int row = 20; row < 40; row++)
    {
        for (int col = 5; col < 20; col++)
        {
            *aMat.ptr<cv::Vec3b>(row, col) = cv::Vec3b(0, 255, 0);
        }
    }
    showImg(aMat);
}

void exercise_4()
{
    cv::Mat aMat = cv::Mat::zeros(cv::Size(210, 210), CV_8UC1);
}

void exercise_5()
{
}

int main()
{
    exercise_3();
    return 0;
}