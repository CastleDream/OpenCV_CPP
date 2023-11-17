/*
cd opencv_code/2.Opencv初探

clang++ -Wall -v -std=c++11 2_6_directionKeyDetect.cpp -lncurses -o ../Execution/2_6_directionKeyDetect

../Execution/2_6_directionKeyDetect 1/3
*/

#include <ncurses.h>

#include <iostream>
#include <termios.h>
#define STDIN_FILENO 0

void linuxDetect()
{
    int ch;

    /* Curses Initialisations */
    initscr();            /* curses initialization */
    keypad(stdscr, TRUE); /* enable KEY_UP/KEY_DOWN/KEY_RIGHT/KEY_LEFT */
    noecho();             /* prevent displaying if other keys */

    printw("Press # to Exit\n");

    while ((ch = getch()) != '#')
    {
        switch (ch)
        {
        case KEY_UP:
            printw("\nUp Arrow");
            break;
        case KEY_DOWN:
            printw("\nDown Arrow");
            break;
        case KEY_RIGHT:
            printw("\nRIGHT Arrow");
            break;
        case KEY_LEFT:
            printw("\nLEFT Arrow");
            break;
        }
    }
}
void virtualKeyDetection()
{
    /*
    并没有找到这方面的内容，大部分都是用下面的实际接收到的字符是由三个字符组成的来判断的
    想用虚拟键来判断的话，可能确实是需要调用事件判断之类的吧。
    Windows实现：
    https://stackoverflow.com/questions/24708700/c-detect-when-user-presses-arrow-key
    中搜索GetStdHandle，可以看到Windows下的VK_LEFT这些键
    */
}

// macos下OK ✅
/*
Enter a character: ^[[A
Your character was UP%
*/
void threeASCII()
{ // Black magic to prevent Linux from buffering keystrokes.
    // 加下面这四行的区别就是：输入上之后不需要/需要加回车，加上的话不用回车，
    // 不加的话需要回车（输入字符多的话也只会判断前三个字符）
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~ICANON;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);

    // Once the buffering is turned off, the rest is simple.
    std::cout << "Enter a character: ";
    char c, d, e;
    std::cin >> c;
    std::cin >> d;
    std::cin >> e;
    std::cout << "\nYour character was ";
    // Using 3 char type, Cause up down right left consist with 3 character
    if ((c == 27) && (d == 91))
    {
        if (e == 65)
        {
            std::cout << "UP";
        }
        if (e == 66)
        {
            std::cout << "DOWN";
        }
        if (e == 67)
        {
            std::cout << "RIGHT";
        }
        if (e == 68)
        {
            std::cout << "LEFT";
        }
    }
}

int main(int argc, char **argv)
{ // https://www.codeproject.com/Questions/1138419/Getting-chars-from-argv
    char kind = *argv[1];
    if (kind == '1')
        linuxDetect(); // 调用了keypad中的KEY_UP等关键词
    if (kind == '2')
        virtualKeyDetection(); // 没有实现
    if (kind == '3')
        threeASCII(); // 拆分实际输入的内容为3个字符进行判断
    return 0;
}