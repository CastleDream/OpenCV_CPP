/*
cd opencv_code/2.Opencv初探

clang++ -Wall -v -std=c++11 2_6_directionKeyDetect.cpp -lncurses -o ../Execution/2_6_directionKeyDetect

../Execution/2_6_directionKeyDetect
*/

#include <ncurses.h>

int main()
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

    return 0;
}