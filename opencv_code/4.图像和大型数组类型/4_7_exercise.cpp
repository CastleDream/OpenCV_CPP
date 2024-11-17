/*
直接F5用.vscode里的配置运行即可
*/
#include <iostream>
#include <opencv2/opencv.hpp>
#define KEY_LEFT 63234
#define KEY_RIGHT 63235
#define KEY_UP 63232
#define KEY_DOWN 63233
/*
创建一个500*500的单通道无符号，数值全为0的图像(ASCII numeric typewriter,所以只能输入0-9这10个字符，转换键也是一个ASCII字符)
1. 可以一直输入ASCII字，每个字都显示为一个20pix高，10pix宽的范围，直到到达图像末尾
2. 可以回车和退格
3. 允许用方向键编辑每个数字
4. 定义一个键，这个键可以把图像转为彩色图，同时每个数字用不同的颜色

https://stackoverflow.com/questions/29411301/opencv-waitkey-method-return-type
opencv也有colormap: <https://docs.opencv.org/4.x/d3/d50/group__imgproc__colormap.html>

退格键删除部分还有点问题，其余差不多了
*/
void exercise_1()
{
    cv::Mat background = cv::Mat::zeros(cv::Size(500, 500), CV_8UC1);
    // cv::Mat numMat = cv::Mat::zeros(cv::Size(25, 50), CV_8UC1);// 原本想用这个记录所有的数字，后来发现没用
    cv::Mat img_color;
    int num = 500 / 20 * (500 / 10);
    int row = 0;
    int col = 0;
    cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE); // HighGUI提供的函数
    int input_key_num;
    cv::Point loc; // 	Bottom-left corner of the text string in the image.
    int fontFace = cv::FONT_HERSHEY_SCRIPT_SIMPLEX;
    int thickness = 2;
    double fontScale = cv::getFontScaleFromHeight(fontFace, 12, thickness);
    int color;
    // 注意，这里fontScale不能直接用高20，否则字体宽度会大于10
    std::string text;
    cv::imshow("Display Image", background);
    for (int i = 0; i < num; i++)
    {
        row = i / 50;
        col = i % 50;
        input_key_num = cv::waitKeyEx(0); // 要支持方向键的话，需要waitKeyEx(waitKey只支持单纯的ASCII)
        // 数字键，只有这个才会有数字出现，其余都是控制，只有这个会产生实际绘制putText
        if ((input_key_num >= 48) && (input_key_num <= 57))
        {
            input_key_num = input_key_num - 48; // 等待键盘输入
                                                // std::cout << "按下的键是数字: " << input_key_num << std::endl;
                                                // 5对应的键码是53 https://www.runoob.com/w3cnote/ascii.html
                                                // 0~9这10个数字与ASCII10进制码的转换是 0是48 即ASCII码-48=对应的数字值。

            // numMat.at<u_int8_t>(row, col) = input_key_num;
            loc.y = row * 20 + 20;
            loc.x = col * 10;
            // std::cout << "num is " << input_key_num << " y is " << loc.y << " x is " << loc.x << std::endl;
            text = std::to_string(input_key_num);
            // 这里为了拉开不同数字的颜色差距，减去20
            color = 255 - input_key_num * 20; // cv::Scalar::all(255 - input_key_num - 20)
            // 先确保当前背景是黑的，再去写字
            background(cv::Range(loc.y - 18, loc.y + 2), cv::Range(loc.x, loc.x + 10 + 1)) = 0;
            cv::putText(background, text, loc, fontFace, fontScale, color, thickness, 8);
            cv::imshow("Display Image", background);
        }
        // 退格键/删除键(左一个), 当前数字就应该消失了
        else if (input_key_num == 127)
        {
            i = i - 1;
            row = i / 50;
            col = i % 50; // 重新确定上一步的位置
            loc.y = row * 20 + 20;
            loc.x = col * 10;
            if ((loc.y >= 0) && (loc.y <= 500) && (loc.x >= 0) && (loc.x <= 500))
            {
                // std::cout << "loc.y is " << loc.y << " loc.x is " << loc.x << std::endl;
                background(cv::Range(loc.y - 20, loc.y + 2), cv::Range(loc.x, loc.x + 10 + 1)) = 0;
                // 因为range是右开，所以这里加个1
                cv::imshow("Display Image", background);
            }
            // cv::putText(background, " ", loc, fontFace, fontScale, cv::Scalar::all(255), thickness, 8);
            // 这个没法删除当前的字，只能通过操作矩阵相应位置，赋值为0来实现了

            i += -1; // 这步相当于没有实质性生效，回退一个,同时i会+1，因此是i-1-1
            // std::cout << "删除键" << std::endl; // 确定DEL键的ASCII字符是127
        }
        // 方向键（和退格键效果差不多） 区别在于不会删除当前字符
        // https://blog.csdn.net/Castlehe/article/details/134234728 3.1 OpenCV检测方向键被按下
        else if (input_key_num == KEY_LEFT)
        {
            i = i - 2; // 除了回去1位，还要减去i在for循环里加的一位
        }
        else if (input_key_num == KEY_RIGHT)
        { // i = i + 1; // 什么都不用执行，i会自动+1的
        }
        // 回车键(下一行)以及下键
        else if ((input_key_num == 13) || input_key_num == KEY_DOWN)
        {
            i += 48; // 但是要回1列，同时由于i自增1，所以50-1-1
        }
        else if (input_key_num == KEY_UP)
        {
            i -= 51; // i-50上一行，但是i会自增，所以要-51
        }
        // q 退出
        else if (input_key_num == 113)
        {
            cv::destroyWindow("Display Image");
            break;
        }
        // c 换彩色
        else if (input_key_num == 99)
        {
            // std::cout << background(cv::Range(6, 20), cv::Range(0, 10)) << std::endl; // 一个数字占据高20 宽10的空间
            applyColorMap(background, img_color, cv::COLORMAP_RAINBOW); //   COLORMAP_JET
            cv::imshow("Display Image", img_color);
        }
    }
}

/*opencv有现成的积分图实现，后面也会讲*/
void exercise_2()
{
}
int main()
{
    exercise_1();
    // exercise_2();
    return 0;
}