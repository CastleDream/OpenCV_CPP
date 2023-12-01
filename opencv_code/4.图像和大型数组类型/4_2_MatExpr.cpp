/*
切换到相应目录
cd ~/Documents/DailyStudy/cpp/opencv_code/4.图像和大型数组类型

clang++ -Wall -v -std=c++11 4_2_MatExpr.cpp \
-I/usr/local/include/opencv4 `pkg-config --libs opencv4` \
-o ../Execution/4_2_MatExpr

运行可执行文件
../Execution/4_2_MatExpr
*/

#include <iostream>
#include <opencv2/opencv.hpp>

void test_create()
{
    cv::Mat m(3, 3, CV_32FC3, cv::Scalar(1, 2, 3));
    // 每个元素是3个浮点数，3channel（但是本质是一个2维数组，只是其中每个数是3个值构成的，访问每个数返回的是三个值）
    // 可以想象一下深度学习里的特征图,3X3的大小，C控制特征图的深度
    // CV_16FC3这系列的数据类型，表示的就是一个3D的矩阵，只是矩阵的深度/厚度可变

    // 而使用std::vector<int>这种构建的是真的多维数组
    cv::Mat m1 = cv::Mat(std::vector<int>({3, 3, 3}), CV_32F, cv::Scalar(1, 2, 3));

    std::cout << "CV_32FC3 \n"
              << m << std::endl;
    std::cout << "Element (2,2) is " << m.at<cv::Vec3f>(2, 2) << std::endl;
    std::cout << "m.dims is " << m.dims << std::endl;

    std::cout << "m1.dims is " << m1.dims << std::endl;
    std::cout << "CV_32F \n"
              << m1 << std::endl;
}

void at_ptr_iterator_compare(int times)
{
    cv::Mat m(512, 1024, CV_32FC3, cv::Scalar(2.0, 4.3, 5.2));
    // at time
    cv::Scalar mean = 0;
    double t = (double)cv::getTickCount();
    for (int i = 0; i < times - 1; i++)
    {
        mean += cv::mean(m.at<cv::Vec3f>(10));
    }
    std::cout << "m.at<cv::Vec3f>(10) is: " << m.at<cv::Vec3f>(10) << std::endl;
    t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
    std::cout << "512*1024 matrix at access one row time is: " << t << std::endl;

    // ptr time
    t = (double)cv::getTickCount();
    for (int i = 0; i < times - 1; i++)
    {
        mean += cv::mean(*m.ptr<cv::Vec3f>(10));
    }
    std::cout << "m.ptr<cv::Vec3f>(10) is: " << m.ptr<cv::Vec3f>(10) << std::endl;
    t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
    std::cout << "512*1024 matrix ptr access one row time is: " << t << std::endl;

    // iterator time
    t = (double)cv::getTickCount();
    cv::MatConstIterator_<cv::Vec3f> it = m.begin<cv::Vec3f>();
    for (int i = 0; i < times - 1; i++)
    {
        mean += cv::mean(*(it += 10));
    }
    std::cout << "*(it += 10) is: " << *(it += 10) << std::endl;
    t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
    std::cout << "512*1024 matrix MatConstIterator_ access one row time is: " << t << std::endl;
}

void test_ptr()
{
    cv::Mat m(4, 4, CV_32FC3);
    cv::randu(m, cv::Scalar(1, 2, 3), cv::Scalar(10, 20, 30));
    std::cout << m << std::endl;

    // 使用ptr成员函数获取指针
    std::cout << "第4行(base_0)第一个元素的第一个通道值的第一个字节对应的内存地址是："
              << m.ptr<cv::Vec3f>(3) << std::endl;
    std::cout << "第4行(base_0)第一个元素值是："
              << *m.ptr<cv::Vec3f>(3) << std::endl;

    cv::Vec3f *p = m.ptr<cv::Vec3f>(3) + 1;
    std::cout << "第4行(base_0)第二个元素的第一个通道值的第一个字节对应的内存地址是："
              << p << std::endl;
    std::cout << "第4行(base_0)第二个元素值是："
              << *p << std::endl;

    // 使用数据指针data和step配合计算
    cv::Vec3f *p2 = (cv::Vec3f *)(m.data + m.step[0] * 3 + m.step[1] * 0);
    std::cout << "第4行(base_0)第一个元素的(另一种计算内存地址的方法)第一个通道值的第一个字节对应的内存地址是："
              << p2 << std::endl;
    std::cout << "第4行(base_0)第一个元素值(另一种计算内存地址的方法)是："
              << *p2 << std::endl;
}
/*
 [5.7725453, 5.5866656, 13.828605, 8.3294659, 9.8683939, 9.7173214, 7.9579458, 15.717688, 11.310452, 7.3218851, 10.61205, 24.38913;
 1.772588, 3.3510847, 7.4124317, 3.6980128, 18.301771, 22.161518, 2.3474126, 15.777919, 6.3555984, 1.0335574, 11.927244, 29.951382;
 2.4309292, 4.9012051, 4.0311117, 4.8327522, 17.230297, 26.965321, 3.3824115, 6.874095, 28.721992, 7.0761814, 16.583895, 7.9669256;
 ⭐️1.8501058, 16.68626, 17.161978, 4.4275494, 9.415906, 9.8994541, 9.5040627, 18.202223, 15.363072, 8.5717621, 7.0454025, 13.406764]
 第4行(base_0)第一个元素的第一个通道值的第一个字节对应的内存地址是：0x7f78ff71e810
 第4行(base_0)第一个元素值是：[1.85011, 16.6863, 17.162]
 第4行(base_0)第二个元素的第一个通道值的第一个字节对应的内存地址是：0x7f78ff71e81c
 第4行(base_0)第二个元素值是：[4.42755, 9.41591, 9.89945]
 第4行(base_0)第一个元素的(另一种计算内存地址的方法)第一个通道值的第一个字节对应的内存地址是：0x7f78ff71e810
 第4行(base_0)第一个元素值(另一种计算内存地址的方法)是：[1.85011, 16.6863, 17.162]
 32/8*3=12个字节，每个元素（由3个数值构成）占12个字节 0 1 2 3 4 5 6 7 8 9 a b c(12)两个内存地址刚好差12
 */

int main(int argc, char **argv)
{
    // test_create();

    // int times = 10000;
    // at_ptr_iterator_compare(times);

    // test_ptr();

    return 0;
}