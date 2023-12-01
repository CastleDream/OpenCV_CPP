/*
切换到相应目录
cd ~/Documents/DailyStudy/cpp/opencv_code/4.图像和大型数组类型

clang++ -Wall -v -std=c++11 4_3_compareNAryMatIterator.cpp \
-I/usr/local/include/opencv4 `pkg-config --libs opencv4` \
-o ../Execution/4_3_compareNAryMatIterator

运行可执行文件
../Execution/4_3_compareNAryMatIterator
*/

#include <iostream>
#include <opencv2/opencv.hpp>
/*
比较两个MatConstIterator_相加赋值给MatIterator_迭代器的速度快，
还是直接只使用一个NAryMatIterator的速度快
*/

using namespace cv;
void testMatIterator(const Mat &src1, const Mat &src2, Mat &dst, int iter)
{
    Size size = src1.size();
    dst.create(size, src1.type());

    double t = (double)getTickCount();
    for (int times = 0; times < iter; times++)
    {
        MatConstIterator_<cv::Vec3b> it1 = src1.begin<cv::Vec3b>(), it1_end = src1.end<cv::Vec3b>();
        MatConstIterator_<cv::Vec3b> it2 = src2.begin<cv::Vec3b>();
        MatIterator_<cv::Vec3b> it3 = dst.begin<cv::Vec3b>();
        for (; it1 != it1_end; ++it1, ++it2, ++it3)
        {
            *it3 = *it1 + *it2;
        }
        // 如果要多次测试，其实每次都需要重新定义指针
    }
    t = ((double)getTickCount() - t) / getTickFrequency();
    double mean_t = t / iter;
    std::cout << "Multi MatIterator time is: " << t << "\n mean time is " << mean_t << std::endl; // Multi MatIterator time is: 0.0128465
    std::cout << "Demo pixel value is:\n"
              << dst(cv::Range(0, 3), cv::Range(0, 3)) << std::endl;
    /*
    Demo pixel value is:
    [255, 255, 255, 254, 255, 255, 252, 255, 255;
    254, 255, 255, 254, 255, 255, 254, 255, 255;
    254, 255, 255, 254, 255, 255, 255, 255, 255]
    */
}

/*
构造函数写法二
这里使用 NAryMatIterator itNAry(arrays, (uchar **)ptrs);
(uchar **)ptrs是NAryMatIterator的另一种构造函数的写法，在Learning opencv3中有示例代码
https://github.com/opencv/opencv/blob/4.x/modules/core/src/count_non_zero.dispatch.cpp#L143
https://github.com/opencv/opencv/blob/4.x/modules/core/src/matrix.cpp#L76
https://github.com/opencv/opencv/blob/4.x/modules/core/src/sum.dispatch.cpp#L206
页面搜索 `uchar* ptrs` 或者uchar，也可以看到类似的使用
*/

void testNAryMatIterator(const Mat &src1, const Mat &src2, Mat &dst, int iter)
{
    const Mat *arrays[] = {&src1, &src2, &dst, NULL};
    cv::Vec3b *ptrs[3];                             // 先定义要迭代的矩阵内部元素的指针
    NAryMatIterator itNAry(arrays, (uchar **)ptrs); // 传递进去的时候强制类型转换为对应的格式

    // 由于我已经人工判断出来这个肯定是连续的，所以没必要写plane的for循环。。
    // std::cout << "itNAry.nplanes is " << itNAry.nplanes << std::endl; // itNAry.nplanes is 1
    // std::cout << "itNAry.size is " << itNAry.size << std::endl;// itNAry.size is 262144    512*512=262144
    double t = (double)getTickCount();
    for (int times = 0; times < iter; times++)
    {
        for (int i = 0; i < itNAry.size; i++)
        {
            ptrs[2][i] = ptrs[0][i] + ptrs[1][i];
            //  鼠标放在ptrs[2][i]会显示 cv::Vec3b *ptrs[3]
            // 如果一开始就用 uchar *ptrs[3];而不是cv::Vec3b *ptrs[3];则显示的就是uchar *ptrs[3]，就无法正确读取image的三通道数据了
        }
        // 但是NAryMatIterator不需要重新指定指针，起码只有1个面的时候并不需要
    }
    t = ((double)getTickCount() - t) / getTickFrequency();
    double mean_t = t / iter;
    std::cout << "NAryMatIterator time is: " << t << "\n mean time is " << mean_t << std::endl; // NAryMatIterator time is: 0.00783082
    std::cout << "Demo pixel value is:\n"
              << dst(cv::Range(0, 3), cv::Range(0, 3)) << std::endl;
    /*Demo pixel value is:
    [255, 255, 255, 254, 255, 255, 252, 255, 255;
    254, 255, 255, 254, 255, 255, 254, 255, 255;
    254, 255, 255, 254, 255, 255, 255, 255, 255]
    */
}

int main(int argc, char **argv)
{
    cv::Mat image1 = cv::imread("../Execution/lena.jpg");
    cv::Mat image2 = cv::imread("../Execution/lena.jpg");
    cv::Mat dst(image1.size(), CV_8UC3), dst2(image1.size(), CV_8UC3);
    std::cout << "image size is " << image1.size << std::endl;   // image size is 512 x 512
    std::cout << "image type is " << image1.type() << std::endl; // image type is 16  8UC3

    int iter = 50;
    testMatIterator(image1, image2, dst, iter);
    // cv::namedWindow("MatIterator");
    // cv::imshow("MatIterator", dst);
    // cv::waitKey(0);

    testNAryMatIterator(image1, image2, dst2, iter);
    // cv::namedWindow("NAryMatIterator");
    // cv::imshow("NAryMatIterator", dst2);
    // cv::waitKey(0);
    bool eq = std::equal(dst.begin<uchar>(), dst.end<uchar>(), dst2.begin<uchar>());
    std::cout << eq << std::endl; // 1(表示两种方法得到的结果相同)
    return 0;
}