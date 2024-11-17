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
    int row;
    cv::Mat ROI;
    cv::Rect roiRect;
    for (int i = 1; i < 11; i++)
    {
        roiRect.x = 10 * i;
        roiRect.y = 10 * i;
        roiRect.width = 210 - 2 * 10 * i;
        roiRect.height = 210 - 2 * 10 * i;
        ROI = aMat(roiRect);
        ROI.setTo(20 * i);
    }
    showImg(aMat);
}

void exercise_5()
{
    cv::Mat image = cv::imread("/Users/huangshan/Documents/DailyStudy/cpp/opencv_code/Execution/lena.jpg");
    cv::Mat roi1 = image(cv::Rect(5, 10, 20, 30));
    cv::Mat roi2 = image(cv::Rect(50, 60, 20, 30));
    cv::bitwise_not(roi1, roi1);
    cv::bitwise_not(roi2, roi2);
    showImg(image);
}

void exercise_6()
{
    cv::Mat image = cv::imread("/Users/huangshan/Documents/DailyStudy/cpp/opencv_code/Execution/lena.jpg");
    // cv::Mat mask;
    // cv::compare(image, cv::Scalar(50, 99, 33), mask, cv::CMP_GT);

    cv::Mat channels[3]; // 0-b  1-g  2-r
    cv::split(image, channels);
    cv::namedWindow("Raw green", cv::WINDOW_AUTOSIZE);
    cv::imshow("Raw green", channels[1]); // 显示绿色通道

    cv::Mat clone1, clone2;
    channels[1].copyTo(clone1); // 深拷贝
    channels[1].copyTo(clone2);

    // 求最大值和最小值, https://answers.opencv.org/question/27248/max-and-min-values-in-a-mat/
    double min, max;
    cv::minMaxLoc(channels[1], &min, &max);
    std::cout << "最大值是: " << max << "最小值是: " << min << std::endl;

    int thresh = (unsigned char)((max - min) / 2);
    // https://stackoverflow.com/questions/20816955/how-to-set-all-pixels-of-an-opencv-mat-to-a-specific-value
    clone1.setTo(cv::Scalar(thresh));
    clone2.setTo(cv::Scalar(0));
    cv::compare(channels[1], clone1, clone2, cv::CMP_GE);
    cv::namedWindow("BiggerThanGreen", cv::WINDOW_AUTOSIZE);
    cv::imshow("BiggerThanGreen", channels[1]); // 显示绿色通道中大于thresh的掩码图像

    cv::subtract(channels[1], thresh / 2, channels[1], clone2);
    cv::namedWindow("SubMiddle", cv::WINDOW_AUTOSIZE);
    cv::imshow("SubMiddle", channels[1]);

    if (113 == cv::waitKey(0)) // q退出
    {
        cv::destroyAllWindows();
    }
}

int main()
{
    exercise_6();
    return 0;
}