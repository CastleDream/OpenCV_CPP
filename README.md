- [1. opencv学习路线](#1-opencv学习路线)
- [2. 文件说明](#2-文件说明)

## 1. opencv学习路线
1. 建议先看python版本的, 可以快速上手了解opencv这个大库,一般都不熟悉opencv的功能和模块组织,
2. 再先看C++的, 适应opencv在C++中的一些数据结构定义和函数调用方法，方便工程部署
3. 针对自己感兴趣的算法，读示例论文, 先用python实现，再用C++实现，然后用python来检查C++的实现

除了opencv库，ITK这类的库也同理，
+ 因为这类库的python版本基本都是基于ctype得到的，是对C++版本的一个封装调用
+ 本质上组织结构还是与C++版本一致的

## 2. 文件说明

```bash
.
├── README.md
├── clang_demo_folder  // 测试clang示例的demo
├── latex // 写C++的cheatsheet使用的latex
├── lib //opencv的源码和build代码，gitignore了
├── opencv_code  // opencv的C++代码
└── opencv_python // opencv的python代码
```

