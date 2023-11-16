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
`2`-1表示是`2`.Opencv初探中的代码
```
.
├── 2_1_displayImage.cpp    // 读取显示图像
├── 2_2_openCamera.cpp      // 使用默认摄像头打开视频流
├── 2_3_readVideo.cpp       // 读取视频文件
├── 2_4_trackBar.cpp        // 添加滑动条控制视频文件读取（像个视频播放器）
└── temp.cpp                // 临时验证一些内容
```