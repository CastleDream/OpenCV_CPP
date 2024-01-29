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
```
.
├── 2_1_displayImage.cpp    // 读取显示图像
├── 2_2_openCamera.cpp      // 使用默认摄像头打开视频流
├── 2_3_readVideo.cpp       // 读取视频文件
├── 2_4_trackBar.cpp        // 添加滑动条控制视频文件读取（像个视频播放器）
├── 2_5_simpleVideoPlayer.cpp // 添加左右方向键和空格键控制视频
├── 2_6_directionKeyDetect.cpp // Windows/linux下检测方向键是否被按下
├── 2_7_directionKeyDetect.cpp // 包括高斯模糊，图像金字塔和边缘检测，获取某个像素值等
├── 2_8_createVideo.cpp     // 转为对数极坐标、源视频和对应的RGB通道三个视角的视频
├── 2_9_exercise.cpp        // 第二章的课后练习-滑动条控制下采样倍数
├── 3_1_BasicDataType.cpp   // 第三章的课后练习，mat,matx和vec的乘法，数据类型一致且必须是浮点数；以及数组的range访问等
├── 3_2_compareMatTime.cpp  // cv::Mat和numpy array矩阵计算速度比较
├── 3_2_compareNumpyTime.py 
├── 4_1_Mat.cpp             // cv::Mat的step的含义，Mat的ptr的指针计算（Mat元素的内存地址计算），at访问
├── 4_2_MatExpr.cpp         // 创建矩阵，at和ptr访问速度比较，ptr访问元素地址计算和头指针偏移
├── 4_3_compareNAryMatIterator.cpp // NAryMatIterator和MatIterator类迭代器访问耗时比较，
                                   // NAryMatIterator构造函数的第二种写法
├── 4_4_NAryMatIteratorDemo.cpp  // NAryMatIterator构造函数的第一种写法，
                                 // NAryMatIterator对多通道，多维度，不连续等的说明
├── 4_5_chunkAcccessMat.cpp     // cv::Mat_的括号重载和cv::Mat的括号重载，以及矩阵比较运算
├── 4_6_sparseMat.cpp           // 稀疏矩阵的一些常见操作
└── temp.cpp                // 临时验证一些内容
```