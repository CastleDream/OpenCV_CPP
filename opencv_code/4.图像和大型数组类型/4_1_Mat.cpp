/*
切换到相应目录
cd opencv_code/4.图像和大型数组类型

clang++ -Wall -v -std=c++11 4_1_Mat.cpp \
-I/usr/local/include/opencv4 `pkg-config --libs opencv4` \
-o ../Execution/4_1_Mat

或者直接指明要链接的库
clang++ -Wall -v -std=c++11 4_1_Mat.cpp \
-I/usr/local/include/opencv4 \
-L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_videoio \
-o ../Execution/4_1_Mat

运行可执行文件
../Execution/4_1_Mat
*/

#include <iostream>
#include <opencv2/opencv.hpp>

void test_step()
{
    cv::Mat mat1(2, 3, CV_8S);
    std::cout << "2d mat step[0] is " << mat1.step[0]
              << "\n step[1] is  " << mat1.step[1]
              << "\n 2d mat size is " << mat1.size
              << "\n 2d mat CV_8S elemSize is " << mat1.elemSize() << std::endl;

    const int n_mat_sz[] = {2, 2, 2};
    cv::Mat mat2(3, n_mat_sz, CV_16S);
    // std::cout << "3d mat demo like: " << mat1 << std::endl;
    // 直接用标准输出流打印3d对象，报错error: (-215:Assertion failed) m.dims <= 2 in function 'FormattedImpl'
    // 因此如果想打印多维数组，需要通过切片，手动划分出2d进行打印
    std::cout << "3d mat step[0] is " << mat2.step[0]
              << "\n step[1] is  " << mat2.step[1]
              << "\n step[2] is  " << mat2.step[2]
              << "\n 3d mat size is " << mat2.size
              << "\n 3d mat CV_16S elemSize is " << mat2.elemSize() << std::endl;
    std::cout << "over step range: mat2.step[3] is " << mat2.step[3] << std::endl;

    const int n_mat_sz2[] = {2, 2, 2, 3};
    cv::Mat mat3(4, n_mat_sz2, CV_32F);
    std::cout << "4d mat step[0] is " << mat3.step[0]
              << "\n step[1] is  " << mat3.step[1]
              << "\n step[2] is  " << mat3.step[2]
              << "\n step[3] is  " << mat3.step[3]
              << "\n 4d mat size is " << mat3.size
              << "\n 4d mat CV_32F elemSize is " << mat3.elemSize() << std::endl;

    // step取值范围是0~M.dims-1
    // rows=2,cols=2的2d矩阵的step是 step[0]=3, step[1]=1
    // 维度为2,2,2（三维，每个维度的个数依次为：2,2,2）的矩阵的step是：step[0]=4, step[1]=2, step[2]=1
}

void test_address()
{
    cv::Mat mat4(2, 3, CV_64F);
    // cv::randn(mat4, 4, 16);
    mat4 = (cv::Mat_<double>(2, 3) << 4, 7, 7, -3, 24, 0);
    std::cout << "mat4 type is " << mat4.type() << " \n mat4 is: \n"
              << mat4 << std::endl;
    std::cout << "mat4[1,2] is " << mat4.at<double>(1, 2) << std::endl;
    // at<schar>(int row, int col)
    std::cout << "2d mat \n step[0] is " << mat4.step[0]
              << "\n step[1] is  " << mat4.step[1]
              << "\n 2d mat size is " << mat4.size
              << "\n 2d mat CV_64F elemSize is " << mat4.elemSize() << std::endl;

    std::cout << "mat4 header point address: " << static_cast<void *>(mat4.data) << std::endl;
    if (mat4.isContinuous())
    {
        double *ptr = reinterpret_cast<double *>(static_cast<void *>(mat4.data));
        for (int i = 0; i < 4; i++)
        {
            *ptr++;
            double value = *ptr;
            std::cout << "偏移后的内存地址：" << ptr << std::endl;
            std::cout << "偏移后的内存数据：" << value << std::endl;
        }
        double *ptr2 = reinterpret_cast<double *>(static_cast<void *>(mat4.data));
        int offset = 3;
        double *newPtr = ptr2 + offset;
        double value = *newPtr;
        std::cout << "偏移 " << offset << " 个数据后的内存地址：" << newPtr << std::endl;
        std::cout << "基于头指针偏移后的内存数据：" << value << std::endl;

        // 直接对内存地址加减，访问即可
        // 如果是多通道的数组，访问方式类似于：
        //(int)(*(mat4.data + mat4.step[0] * row + mat4.step[1] * col+channel))
        // row相当于y，col相当于x，channel相当于z
        double *calPtr = reinterpret_cast<double *>(mat4.data + mat4.step[0] * 1 + mat4.step[1] * 1);
        double *calPtr2 = (double *)(mat4.data + mat4.step[0] * 1 + mat4.step[1] * 1);

        std::cout << "直接用内存公式计算来访问, mat4[1,1]的内存地址: " << calPtr
                  << "\n 对应的值是：" << *calPtr
                  << "\n 对应的值(另一种转换方式)是：" << *calPtr2 << std::endl;
    }
    else
    {
        std::cout << "mat4 is not continuous" << std::endl;
    }
}

/*
很奇怪，signed char无法直接用cout进行输出,

std::cout << "mat5[1,2] is " << mat5.at<schar>(1, 2) << std::endl;
这句输出为空

这是因为 std::cout不支持signed char，可以点开cout的函数看看

一般都是强制类型转换成int或者short来进行显示的
https://cboard.cprogramming.com/cplusplus-programming/178728-cout-unsigned-char.html
 https://stackoverflow.com/questions/48275998/cout-of-negative-value-of-signed-char-in-c
 */
void test_schar_out()
{
    cv::Mat mat5(2, 3, CV_8S);
    cv::randn(mat5, 4, 16);
    std::cout << "mat5 type is " << mat5.type() << " \n mat5 is: \n"
              << mat5 << std::endl;
    if (mat5.isContinuous())
    {
        std::cout << "mat5[1,2] is " << (short)mat5.at<schar>(1, 2) << std::endl;
    }
}
void test_at()
{
    // 单通道访问
    cv::Mat m = cv::Mat::eye(10, 10, CV_32FC1);
    std::cout << "Element (3,3) is " << m.at<float>(3, 3) << std::endl;

    // 多通道访问
    cv::Mat m2 = cv::Mat::eye(10, 10, CV_32FC3);
    // In case of multi-channels type, identity matrix will be initialized only for the first channel, the others will be set to 0's
    // eye函数如果用于多通道类型，那么只会对第一个通道进行初始化，其余通道都是0
    std::cout << "Element (3,3) is " << m2.at<cv::Vec3f>(3, 3) << std::endl;
}
void test_bracket()
{
    cv::Mat_<cv::Vec<float, 1>> m(10, 10);
    m = cv::Mat::eye(10, 10, CV_32FC1);
    std::cout << "Element (3,3) is " << m(3, 3) << std::endl;
}
int main(int argc, char **argv)
{
    // test_step();
    // test_address();
    // test_at();
    test_bracket();
    return 0;
}
