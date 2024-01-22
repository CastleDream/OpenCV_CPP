/*
@File    :   4_6_sparseMat.cpp
@Time    :   2024/01/20 22:27:10
@Author  :   xxx
@Version :   1.0
@Contact :   XXX@outlook.com
@License :   (C)Copyright 2023~
@Desc    :   None

切换到相应目录
cd ~/Documents/DailyStudy/cpp/opencv_code/4.图像和大型数组类型

clang++ -Wall -v -std=c++11 4_6_sparseMat.cpp \
-I/usr/local/include/opencv4 `pkg-config --libs opencv4` \
-o ../Execution/4_6_sparseMat

运行可执行文件
../Execution/4_6_sparseMat
*/

#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char **argv)
{ // 1. 构造一个稀疏矩阵
    int size[] = {10, 10};
    cv::SparseMat sm(2, size, CV_32F);
    for (int i = 0; i < 10; i++)
    {
        int idx[2];
        idx[0] = size[0] * rand();
        idx[1] = size[1] * rand();
        std::cout << "i is " << i << " idx[0] is " << idx[0] << ", idx[1] is " << idx[1] << std::endl;
        sm.ref<float>(idx) += 1.0f;
    }

    // 2. 打印非0元素
    cv::SparseMatConstIterator_<float> it_begin = sm.begin<float>();
    cv::SparseMatConstIterator_<float> it_end = sm.end<float>();
    for (; it_begin != it_end; ++it_begin)
    {
        const cv::SparseMat::Node *node = it_begin.node();
        std::cout << "node->idx[0]: " << node->idx[0] << " node->idx[1]: " << node->idx[1] << " " << *it_begin << std::endl;
    }
    return 0;
}