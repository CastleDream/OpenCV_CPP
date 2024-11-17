#include <iostream>
#include "one.h"

void test_getNewFilePath()
{
    char *png_img_path = (char *)"I:\\517_1280_valid\\1_20180525165039M28L_fl.mwi.png";
    std::cout << "raw path is: " << png_img_path << std::endl;
    char eye = 'L';
    png_img_path = getNewFilePath(png_img_path, 1, eye);
    std::cout << "changed path is: " << png_img_path << std::endl;
}

int main()
{
    test_getNewFilePath();
    return 0;
}