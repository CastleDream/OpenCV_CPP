- [1. 文件结构说明](#1-文件结构说明)

# 1. 文件结构说明
```bash
tree -L 2
.
├── README.md
├── inc                         # 放所有的头文件，名称与src中的文件对应
│   └── string_char.h           
├── result                      # 存放运行结果，这个没有上传，与.vscode里的配置一致
├── src                         # 源文件
│   └── string_char.cpp         # 关于std:string的相关使用，以及部分C风格字符串转换
└── test.cpp                    # 功能函数的测试文件，初期只有一个，后续视情况添加
```