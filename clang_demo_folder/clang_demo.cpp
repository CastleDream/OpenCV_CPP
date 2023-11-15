#include <iostream>
#include "max.h"
#define STR "Hello world"

int main(int argc, const char *argv[])
{
    std::string a = STR;
    std::cout << a << std::endl;
    int max_num = max(10, 20);
    std::cout << "最大值是" << std::to_string(max_num) << std::endl;
    return 0;
}