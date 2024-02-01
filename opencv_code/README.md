## 文件说明

```bash
.
├── 2.Opencv初探     // 第二章的代码
├── 3.OpenCV数据类型  // 第三章的代码
├── 4.图像和大型数组类型  // 第四章的代码
├── README.md
├── CMakeLists.txt  //这下面的都是测试CMake编译以及clang进行debug的内容
├── DisplayImage
├── DisplayImage.cpp
├── DisplayImageDebug
├── DisplayImageDebug.dSYM
└── build
```

## 运行说明
+ 规整的书中的代码都会放在对应的章节里，使用`clang++`运行，不使用`CMakeLists.txt`
+ 每个cpp源文件的运行方式都以注释形式，写在每个源文件的最上面。


## 代码内容说明
`2`-x表示是`2`.Opencv初探中的代码，以此类推

| 章节                 | 脚本名称                       | 功能/完成内容                                                                       |
| :------------------- | :----------------------------- | :---------------------------------------------------------------------------------- |
| 2.Opencv初探         | 2_1_displayImage.cpp           | 1. 读取显示图像<br/> 2.运行opencv的example的命令                                    |
|                      | 2_2_openCamera.cpp             | 使用默认摄像头打开视频流                                                            |
|                      | 2_3_readVideo.cpp              | 读取视频文件                                                                        |
|                      | 2_4_trackBar.cpp               | 1.添加滑动条控制视频文件读取（像个视频播放器）<br/> 2.支持单步和连续播放            |
|                      | 2_5_simpleVideoPlayer.cpp      | 比2.4多了左右和空格键的控制                                                         |
|                      | 2_6_directionKeyDetect.cpp     | Windows/linux下检测方向键是否被按下                                                 |
|                      | 2_7_simpleTransformer.cpp      | 1.高斯模糊<br/>2.图像金字塔<br/>3.边缘检测<br/>4.获取某个像素值等（at访问矩阵）     |
|                      | 2_8_createVideo.cpp            | 创建转为对数极坐标、源视频和对应的RGB通道三个视角的视频                             |
|                      | 2_9_exercise.cpp               | 第二章的课后练习-滑动条控制下采样倍数                                               |
|                      | 2_10_other.cpp                 | 1.获取opencv build的信息<br/> 2.waitkey按键和highGUI的关系                          |
| 3.OpenCV数据类型     | 3_1_BasicDataType.cpp          | 第三章的课后练习，mat等的乘法;数组的range访问等                                     |
|                      | 3_2_compareMatTime.cpp         | 512*1024矩阵乘法使用C++ OpenCV的Mat的速度                                           |
|                      | 3_2_compareNumpyTime.py        | 512*1024矩阵乘法使用python的Numpy的Array的速度                                      |
| 4.图像和大型数组类型 | 4_1_Mat.cpp                    | 1.cv::Mat的step的含义<br/>2.Mat的ptr指针计算（Mat元素的内存地址计算）<br/>3.at访问  |
|                      | 4_2_MatExpr.cpp                | 1.创建矩阵<br/>2.at和ptr访问速度比较<br/>3.ptr访问元素地址计算和头指针偏移          |
|                      | 4_3_compareNAryMatIterator.cpp | 1.NAryMatIterator和MatIterator比较<br/>2. NAryMatIterator构造函数用法1              |
|                      | 4_4_NAryMatIteratorDemo.cpp    | 1.NAryMatIterator构造函数用法2<br/> 2.NAryMatIterator对多通道，多维度，不连续等说明 |
|                      | 4_5_chunkAcccessMat.cpp        | 1.cv::Mat_的括号重载<br/>2.cv::Mat的括号重载<br/>3.矩阵比较运算                     |
|                      | 4_6_sparseMat.cpp              | 稀疏矩阵的一些常见操作                                                              |
| 5.矩阵操作           | 5_1_matOperator.cpp            | 1. bitwise()函数使用<br/>2.  calcCovarMatrix函数说明                                |
