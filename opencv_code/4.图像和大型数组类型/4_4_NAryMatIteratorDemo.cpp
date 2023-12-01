/*
切换到相应目录
cd ~/Documents/DailyStudy/cpp/opencv_code/4.图像和大型数组类型

clang++ -Wall -v -std=c++11 4_4_NAryMatIteratorDemo.cpp \
-I/usr/local/include/opencv4 `pkg-config --libs opencv4` \
-o ../Execution/4_4_NAryMatIteratorDemo

运行可执行文件
../Execution/4_4_NAryMatIteratorDemo

整体说明：把下面的例子和对应的文字描述看一遍，基本就完全理解这个NAryMatIterator了
*/

#include <iostream>
#include <opencv2/opencv.hpp>

// 对一个多维矩阵求和，即：一个多维矩阵的所有元素求和
// 这里处理的矩阵是连续的，只有1个plane
// 🌈 1. 单个连续矩阵求和
void sum_multidimensional()
{
    // 定义一个3维矩阵，5*5*5，由于是在0~1之间的均匀分布，因此和应该是125/2=62.5左右
    const int n_mat_size = 5;
    const int n_mat_sz[] = {n_mat_size, n_mat_size, n_mat_size};
    cv::Mat n_mat(3, n_mat_sz, CV_32FC1);

    // 用均匀分布给矩阵赋值
    cv::RNG rng; // Random Number Generator
    rng.fill(n_mat, cv::RNG::UNIFORM, 0.0, 0.1);

    // 初始化NAryMatIterator
    const cv::Mat *arrays[] = {&n_mat, NULL};
    cv::Mat planes[1];
    std::cout << "planes is " << *planes << std::endl;
    cv::NAryMatIterator it(arrays, planes);

    // 迭代求和
    float s = 0.f;
    cv::Scalar s2(0, 0, 0, 0);

    // size_t it.nplanes ——  typedef unsigned long size_t
    // 不是在OpenCV里搜到的 https://docs.opencv.org/4.x/d3/d3f/classcv_1_1Allocator.html#a23e0cc13f851ac190d768b12c3dae3b1
    // typedef size_t cv::Allocator<_Tp>::size_type
    std::cout << "nplanes is " << it.nplanes << std::endl; // nplanes is 1
    std::cout << "n_mat 是否连续： " << n_mat.isContinuous() << std::endl;

    /*
    每次迭代时，itNAry.planes[i](Mat类型)都会被设置为传递给NAryMatIterator构造器的第i个n维矩阵。
    对于本例来说，由于传递给NAryMatIterator迭代器的构造函数的数组列表为： {&n_mat, NULL}
    因此，i的取值只有0，只有1个n维矩阵。

    区别就在于，相对于多个MatIterator_迭代器，for循环的时候，要对多个it进行++it，但是这里只需要对一个进行++it。
    另外，这里的++it表示继续到每个要迭代的矩阵的一下个面
    */
    for (size_t p = 0; p < it.nplanes; p++, ++it)
    {
        s += cv::sum(it.planes[0])[0];
        /* sum src – input array that must have from 1 to 4 channels
        The function cv::sum calculates and returns the sum of array elements, independently for each channel.*/
        s2 += cv::sum(it.planes[0]);
    }
    std::cout << "sum is: " << s << "\n s2 is: " << s2 << std::endl;
    // 输出为：  sum is: 6.26319   s2 is : [ 6.26319, 0, 0, 0 ]

    // cv::Mat n_mat(3, n_mat_sz, CV_32FC1); 改为 cv::Mat n_mat(3, n_mat_sz, CV_32FC3);
    // 则输出为：sum is: 6.4294 s2 is : [ 6.4294, 6.3359, 6.24881, 0 ]

    s = 0;
    s = cv::sum(it.planes[0])[0];
    s2 = cv::sum(it.planes[0]);
    std::cout << "不进行for循环, sum is: " << s << "\n s2 is: " << s2 << std::endl;
    // 输出为： 不进行for循环, sum is: 6.26319   s2 is : [ 6.26319, 0, 0, 0 ]
    // 所以其实上面的for循环只运行了一次，完全没有循环起来，因为it.nplanes=1
}

// 整体内容类似sum_multidimensional，但是这里用来求和的矩阵是不连续的
// 🌈 2.三通道的单个不连续矩阵求和（为了看清sum）
void sum_multidimensional2()
{
    cv::Mat n_mat(3, 3, CV_32FC3, cv::Scalar(1, 2, 3));
    cv::Mat n_range = n_mat(cv::Range(0, 2), cv::Range(0, 2));
    std::cout << "n_range is \n"
              << n_range << std::endl;
    /*
                ---0---  ---1---
    n_range is [1, 2, 3, 1, 2, 3;
                1, 2, 3, 1, 2, 3]
    第一行是个内存连续的plane（1,2,3是一个元素，这是3通道的矩阵），第二行是个内存连续的plane
    */
    const cv::Mat *arrays[] = {&n_range, NULL};
    cv::Mat planes[1]; // planes是 cv::Mat *类型，是个指针，会在迭代过程中，用每个迭代矩阵的plane对其进行赋值
    cv::NAryMatIterator it(arrays, planes);
    cv::Scalar s;
    std::cout << "nplanes is " << it.nplanes << std::endl;                     // nplanes is 2
    std::cout << "n_range 是否连续： " << n_range.isContinuous() << std::endl; // n_range 是否连续： 0

    /*
     对于单个矩阵来说，nplanes就是这个矩阵的不连续的内存的个数，++it其实就是迭代不连续的内存的个数
     it.planes[0]  因为初始化cv::NAryMatIterator it的arrays里只有1个矩阵，所以it.planes[]的列表长度只有1，索引只有0
     */
    for (size_t p = 0; p < it.nplanes; p++, ++it)
    {
        s += cv::sum(it.planes[0]);
        std::cout << "第 " << p << " 次的it.planes[0]是： " << it.planes[0] << std::endl;
    }
    std::cout << "sum is: " << s << std::endl; // sum is: [4, 8, 12, 0]

    s = cv::sum(it.planes[0]);
    std::cout << "不进行for循环, sum is: " << s << std::endl; // 不进行for循环, sum is: [2, 4, 6, 0]
}

// 🌈 2.单通道的单个不连续矩阵求和（为了看清planes）
void sum_multidimensional3()
{
    cv::Mat n_mat = (cv::Mat_<uchar>(3, 3) << 1, 2, 3, 4, 5, 6, 7, 8, 9);
    cv::Mat n_range = n_mat(cv::Range(0, 2), cv::Range(0, 2));
    std::cout << "n_range is \n"
              << n_range << std::endl;
    // n_range is [ 1, 2;
    //             4, 5 ]
    const cv::Mat *arrays[] = {&n_range, NULL};
    cv::Mat planes[1]; // planes是 cv::Mat *类型，是个指针，会在迭代过程中，用每个迭代矩阵的plane对其进行赋值
    cv::NAryMatIterator it(arrays, planes);
    cv::Scalar s;
    std::cout << "nplanes is " << it.nplanes << std::endl;                     // nplanes is 2
    std::cout << "n_range 是否连续： " << n_range.isContinuous() << std::endl; // n_range 是否连续： 0

    for (size_t p = 0; p < it.nplanes; p++, ++it)
    {
        s += cv::sum(it.planes[0]);
        std::cout << "第 " << p << " 次的it.planes[0]是： " << it.planes[0] << std::endl;
    }
    // 第 0 次的it.planes[0]是： [  1,   2]
    // 第 1 次的it.planes[0]是： [  4,   5]

    std::cout << "sum is: " << s << std::endl; // sum is: [12, 0, 0, 0]

    s = cv::sum(it.planes[0]);
    std::cout << "不进行for循环, sum is: " << s << std::endl;
    // 不进行for循环, sum is: [9, 0, 0, 0]，算的是第二个plane的和
}

// 将两个矩阵对应元素相加，得到一个新的矩阵
// 🌈 3.两个连续矩阵求和
void two_Mat_Sum()
{
    const cv::Mat mat1 = cv::Mat::ones(3, 3, CV_8UC1); // 创建一个全为1的矩阵
    const cv::Mat mat2 = (cv::Mat_<uchar>(3, 3) << 1, 2, 3, 4, 5, 6, 7, 8, 9);

    cv::Mat result(3, 3, CV_8UC1, cv::Scalar(0));          // 创建一个用于存储结果的矩阵
    cv::Mat planes[3];                                     // mat类型的数组，其中每一个对应*mats里的一个矩阵
    const cv::Mat *mats[] = {&mat1, &mat2, &result, NULL}; // 创建一个矩阵数组

    cv::NAryMatIterator it(mats, planes, 3);                  // 创建一个NAryMatIterator对象
    std::cout << "it.nplanes is " << it.nplanes << std::endl; // it.nplanes is 1

    /*
    it.size:  the size of each segment (in elements)
    指明每一个面的大小，不考虑通道数。
    */

    for (size_t i = 0; i < it.nplanes; i++, ++it)
    {
        // cv::uint8_t *mat1_ptr = it.planes[0].ptr<uchar>();
        // cv::uint8_t *mat2_ptr = it.planes[1].ptr<uchar>();
        // cv::uint8_t *result_ptr = it.planes[2].ptr<uchar>();
        cv::uint8_t *mat1_ptr = planes[0].ptr<uchar>();
        cv::uint8_t *mat2_ptr = planes[1].ptr<uchar>();
        cv::uint8_t *result_ptr = planes[2].ptr<uchar>();
        // 这两种写法是一样的

        for (size_t j = 0; j < it.size; j++)
        {
            result_ptr[j] = mat1_ptr[j] + mat2_ptr[j];
        }
    }
    std::cout << result << std::endl;
    // 输出为
    //[ 2, 3, 4;
    // 5, 6, 7;
    //  8, 9, 10 ]
}

// 🌈 4.两个不连续的且多通道的矩阵求和
void two_Mat_Sum2()
{
    cv::Mat mat1 = cv::Mat::ones(4, 4, CV_8UC3);
    cv::Mat mat2(4, 4, CV_8UC3);
    cv::RNG rng;
    rng.fill(mat2, cv::RNG::UNIFORM, 1, 10);

    cv::Mat range1 = mat1(cv::Range(0, 3), cv::Range(0, 3));
    cv::Mat range2 = mat2(cv::Range(0, 3), cv::Range(0, 3));
    cv::Mat result(3, 3, CV_8UC3, cv::Scalar(0));
    std::cout << "range1 is \n"
              << range1 << std::endl;
    std::cout << "range2 is \n"
              << range2 << std::endl;

    const cv::Mat *arrays[] = {&range1, &range2, &result, NULL};
    // arrays里同时进行迭代的数组，尺寸，大小，以及面的个数（每个面里的元素数量）都要求是一致的
    cv::Mat planes[3];
    // 构造函数写法一
    cv::NAryMatIterator it(arrays, planes, 3);

    std::cout << "it.nplanes is " << it.nplanes << std::endl; //
    for (size_t p = 0; p < it.narrays; p++, ++it)
    {
        // it.planes[0] ~it.planes[2] 对应的就是三个矩阵
        //++it,对应的是每个矩阵的不同的面
        // 注意，这里访问利用it.size访问数据的时候，只会考虑高和宽，不会考虑深度（通道数），想要得到正确的通道数，需要构建正确的ptr

        cv::Vec3b *mat1_ptr = it.planes[0].ptr<cv::Vec3b>();
        cv::Vec3b *mat2_ptr = it.planes[1].ptr<cv::Vec3b>();
        cv::Vec3b *result_ptr = it.planes[2].ptr<cv::Vec3b>();
        // cv::Vec3b *mat1_ptr = planes[0].ptr<cv::Vec3b>();
        // cv::Vec3b *mat2_ptr = planes[1].ptr<cv::Vec3b>();
        // cv::Vec3b *result_ptr = planes[2].ptr<cv::Vec3b>(); // 这两种获取矩阵指针的结果是一样的
        std::cout << "第 " << p << " 个面的it.size是:  " << it.size << std::endl;

        for (size_t j = 0; j < it.size; j++)
        {
            // 这里是每个面中的元素
            result_ptr[j] = mat1_ptr[j] + mat2_ptr[j];
            // *(&result_ptr[j] + 1) = *(&mat1_ptr[j] + 1) + *(&mat2_ptr[j] + 1);
            // *(&result_ptr[j] + 2) = *(&mat1_ptr[j] + 2) + *(&mat2_ptr[j] + 2);
        }
    }
    std::cout << result << std::endl;
}
/*
如果使用 cv::uint8_t *mat1_ptr = it.planes[0].ptr<uchar>();
则得到的输出，三个元素刚好对应第一个元素的三个通道
[  9,   6,   5,   0,   0,   0,   0,   0,   0;
   2,   6,   1,   0,   0,   0,   0,   0,   0;
   5,   7,   2,   0,   0,   0,   0,   0,   0]
此时配上
*(&result_ptr[j] + 1) = *(&mat1_ptr[j] + 1) + *(&mat2_ptr[j] + 1);
*(&result_ptr[j] + 2) = *(&mat1_ptr[j] + 2) + *(&mat2_ptr[j] + 2);
就会得到
[  9,   6,   5,   4,   5,   0,   0,   0,   0;
   2,   6,   1,   8,   9,   0,   0,   0,   0;
   5,   7,   2,   9,   7,   0,   0,   0,   0]

即因为ptr的类型错误，每次只取一个元素的一个channel

如果使用cv::Vec3b *mat1_ptr = it.planes[0].ptr<cv::Vec3b>(); 构建的ptr是访问三通道的，则就会得到正确输出：
[  9,   6,   5,   4,   5,   8,   7,   3,   8;
   2,   6,   1,   8,   9,   9,   8,   1,   8;
   5,   7,   2,   9,   7,   6,   7,   8,   8]
*/

void continuous()
{
    // 一般直接创建的变量都是连续的
    // 用切片的方式新创建的变量，只是指针指向原来的数据，因此对于新创建变量的数据，其内存并不是连续的
    cv::Mat m1 = (cv::Mat_<float>(3, 3) << 1, 2, 3, 4, 5, 6, 7, 8, 9);
    cv::Mat m2 = m1(cv::Range(0, 2), cv::Range(0, 2));
    std::cout << "m1的连续性: " << m1.isContinuous() << std::endl; // 输出 m1的连续性: 1
    std::cout << "m2的连续性: " << m2.isContinuous() << std::endl; // 输出 m2的连续性: 0

    // 常规的C++的数组，即便只有1个元素也不需要额外写一个"，"或者别的
    int billy[] = {16, 2, 77, 40, 12071};
    int billy2[] = {1};
    std::cout << "billy is " << *billy << "\n "   // 输出 billy is 16
              << "billy2 is " << *billy2 << "\n"; // 输出 billy2 is 1

    const cv::Mat *arrays[] = {&m2, 0};
    // const cv::Mat *arrays[] = {&m2};
    // 报错： [1]    42471 segmentation fault  ../Execution/4_4_NAryMatIteratorDemo

    cv::Mat planes[3];

    /* https://docs.opencv.org/4.x/d5/dd2/classcv_1_1NAryMatIterator.html#a001ac5692124eefacd223ab4c26a430f
    初始化迭代器（与STL初始化的方式不同），初始化后，迭代器会包含将要迭代的面的数量，
    `it`表示的NAryMatIterator迭代器会同时增加多个矩阵的iterators，其中这些矩阵是以一个null结尾的列表的指针表示的

    因此会看到, cv::NAryMatIterator的构造函数的第一个参数 const Mat ** arrays 的常见形式：
    1. const cv::Mat *arrays[] = {&m2, 0};  // 以0作为矩阵列表的最后一个元素
        例如：https://docs.opencv.org/4.x/d5/dd2/classcv_1_1NAryMatIterator.html#a001ac5692124eefacd223ab4c26a430f中的示例
        https://github.com/oreillymedia/Learning-OpenCV-3_examples/blob/master/example_04-01.cpp
    2. const Mat      *arrays[] = {&src, NULL}; //以NULL作为矩阵列表的最后一个元素
        Mat            planes[1];
        NAryMatIterator it(arrays, planes, 1);
        例如：https://github.com/opencv/opencv/blob/4.x/modules/core/src/count_non_zero.dispatch.cpp
    */
    cv::NAryMatIterator it(arrays, planes);
    std::cout << it.nplanes << ", " << it.planes[0].rows << ", " << it.planes[0].cols << std::endl;
    // 输出 2, 1, 2
    // 即m2这个通过对m1进行截取得到的矩阵，是有两个连续的内存区域（两个"面"）的。
    // 第0行的第0和第1个元素(连续区域1)，以及第1行的第0和第1个元素（连续区域2）
}

int main(int argc, char **argv)
{

    // continuous();
    // sum_multidimensional();
    // sum_multidimensional2();
    // sum_multidimensional3();
    // two_Mat_Sum();
    two_Mat_Sum2();
    return 0;
}