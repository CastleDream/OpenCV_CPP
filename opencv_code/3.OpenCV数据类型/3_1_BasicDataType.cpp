/*
切换到相应目录
cd opencv_code/3.OpenCV数据类型

clang++ -Wall -v -std=c++11 3_1_BasicDataType.cpp \
-I/usr/local/include/opencv4 `pkg-config --libs opencv4` \
-o ../Execution/3_1_BasicDataType

或者直接指明要链接的库
clang++ -Wall -v -std=c++11 3_1_BasicDataType.cpp \
-I/usr/local/include/opencv4 \
-L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_videoio \
-o ../Execution/3_1_BasicDataType

运行可执行文件
../Execution/3_1_BasicDataType
*/

#include <iostream>
#include <opencv2/opencv.hpp>

void test()
{
    cv::Scalar a;
    a = a.ones();
    std::cout << a[0] << ", " << a[1] << ", " << a[2] << ", " << a[3] << ", " << std::endl;

    cv::Matx31f m = {1, 2, 3};
    cv::Matx33f matx331f = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    cv::Matx31f matx31f = matx331f.diag(); // 把对角元素放到一个matx中
    // inline float &cv::Matx31f::operator()(int row, int col)
    /*
    template<typename _Tp, int m, int n> class Matx
    {
    public:
        enum {
            rows     = m,
            cols     = n,
    Matx(rows,cols)
    cv::Matx31f就是3行1列了。
    */
    std::cout << "diag output: \n"
              << matx31f(0, 0) << ", " << matx31f(1, 0) << ", " << matx31f(2, 0) << ", " << std::endl;
    // 输出 2,5,8

    cv::Point begin(1, 2);
    cv::Size oneRec(1, 2);
    cv::Scalar oneScalar(1, 2, 3, 4);
    cv::Rect2i oneRect(begin, oneRec);

    cv::Vec2i beginVec;
    cv::Vec4i oneScalaVec;
    beginVec = (cv::Vec2i)begin;
    // beginVec = (cv::Vec2i)oneRec; // 不存在用户定义的从 "cv::Size" 到 "cv::Vec2i" 的适当转换
    oneScalaVec = (cv::Vec4i)oneScalar;
    // oneScalaVec = (cv::Vec4i)oneRect; // 不存在用户定义的从 "cv::Rect2i" 到 "cv::Vec4i" 的适当转换

    // cv::Matx31f m31f = cv::Matx33f::diag();
    cv::Matx33f matx33f = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::cout << "Once access one member " << matx33f(1, 2) << std::endl;
    matx33f.row(1); // 提取第二行，base 0
    std::cout << "Once access one row " << matx33f.row(1) << std::endl;
    // 跨行，跨列提取
    // get_minor—— extract part of the matrix
    cv::Matx22f matx22f = matx33f.get_minor<2, 2>(1, 1);
    std::cout << "Once access row and line(Matx) " << matx33f.get_minor<2, 2>(0, 0) << std::endl;
    std::cout << "Once access row and line(Matx) " << matx22f << std::endl;

    cv::Mat mat({1, 2, 3, 4, 5, 6, 7, 8, 9}); // mat跨行跨列提取
    std::cout << "mat size:" << mat.size << std::endl;
    std::cout << "Once access row and line(Max) " << mat(cv::Range(0, 4), cv::Range(0, 1)) << std::endl;
}

void exercise_1()
{
    float a = -0.4;
    // float a_abs = cv::abs(a);
    std::cout << "abs: " << std::abs(a)
              << " , round: " << std::round(a)
              << " , Round up: " << std::ceil(a)
              << " , Round down: " << std::floor(a) << std::endl;

    cv::Matx33f random_value = cv::Matx33f::randn(1.0, 0.0);
    std::cout << "random normal distribution: " << random_value << std::endl;
    //[1, 1, 1; 1, 1, 1; 1, 1, 1]

    cv::Point2f point2f(2.4, 3.0);
    std::cout << "convert float to int: float is  " << point2f
              << " , int is " << (cv::Point)point2f << std::endl;
    cv::Point point2i(2, 3);
    std::cout << "convert int to float: int is  " << point2i
              << " , float is " << (cv::Point2f)point2i << std::endl;
}

void exercise_2()
{
    cv::Matx33f random_matx = cv::Matx33f::randu(1, 10);
    cv::Vec3f random_vec = cv::Vec3f::randn(1.0, 0.0);
    std::cout << "exercise_2 \n random_matx is " << random_matx
              << "\n random_vec is " << random_vec
              << "\n random_matx * random_vec rs is " << random_matx * random_vec << std::endl;
    //   << "\n random_vec * random_matx rs is " << random_vec * random_matx

    // 函数 "cv::operator*(const cv::Affine3d &affine, const cv::Vec3d &v)" 不匹配，因为参数 #1 与参数不匹配
    // https://docs.opencv.org/3.4/d0/de1/group__core.html#gac57f9a35a5d4d1d3ba23d7871113cbd3
    // static Affine3<T> cv::operator*(const Affine3<T> &affine1,const Affine3<T> &affine2)
    // 只能matx*vec，不能vec*matx
}

void exercise_3()
{
    // 报错： type == B.type() in function 'gemm' random_mat * random_vec rs is [1]
    // 进行乘法的两个数据类型必须一致
    cv::Mat random_mat = cv::Mat::ones(3, 3, CV_32F);
    cv::Vec3f random_vec(1, 2, 3);
    // cv::Mat casted_result = (cv::Mat)random_vec;
    // 不能用这种方式，还是报错 type == B.type() in function 'gemm'
    cv::Mat casted_result(random_vec, CV_32F);
    std::cout << "exercise_3 \n random_mat is " << random_mat
              << "\n random_vec is " << random_vec
              << "\n random_mat * random_vec rs is " << random_mat * random_vec
              << "\n random_mat * (cv::Mat) random_vec rs is " << random_mat * (cv::Mat)random_vec
              << "\n random_mat * casted_result rs is " << random_mat * casted_result << std::endl;

    // error: (-215:Assertion failed) a_size.width == len in function 'gemm'
    // << "\n casted_result * random_mat rs is " << casted_result * random_mat
    // 报错，和matx一样，只支持只能mat*vec，不能vec*mat
    // mat可以直接*vec，不需要类型转换
}
/*
OpenCV里的矩阵乘法，是使用gemm的，而gemm，只支持浮点数乘法，CV_32FC1, CV_64FC1
https://docs.opencv.org/4.x/d2/de8/group__core__array.html#gacb6e64071dffe36434e1e7ee79e7cb35
https://docs.opencv.org/4.x/d9/d88/group__cudaarithm__arithm.html#ga42efe211d7a43bbc922da044c4f17130
https://stackoverflow.com/questions/15049811/opencv-error-multiplying-2-mats


CV_32FC1, CV_64FC1

*/

int main(int argc, char **argv)
{
    // test();
    // exercise_1();
    exercise_2();
    exercise_3();
    return 0;
}