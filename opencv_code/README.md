## 文件说明

```bash
.
├── 2.Opencv初探  // 第二章的代码
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
`2`-x表示是`2`.Opencv初探中的代码
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
├── 2_9_exercise.cpp        // 课后练习-滑动条控制下采样倍数
├── 3_1_BasicDataType.cpp   // 课后练习，mat,matx和vec的乘法，数据类型一致且必须是浮点数；以及数组的range访问等
├── 3_2_compareMatTime.cpp  // cv::Mat和numpy array矩阵计算速度比较
├── 3_2_compareNumpyTime.py 
└── temp.cpp                // 临时验证一些内容
```