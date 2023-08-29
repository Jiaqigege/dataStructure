#include <ncurses.h>
#include <stdio.h>

#include "display.h"

void display_init(void)
{
    initscr();
    noecho();
    cbreak();
    raw();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
}

void display_deinit(void)
{
    refresh();
    endwin();
}

void display_refresh(void)
{
    refresh();
}

void display_clear(void)
{
    clear();
}

void display_point(int x, int y, uint8_t point)
{
    mvaddch(y, x, point);
}

static int test(void)
{
    display_init();

    int width = 80;
    int height = 24;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // 一个简单的模式：如果 x 和 y 都是偶数或奇数，则绘制一个像素
            if ((x % 2 == 0 && y % 2 == 0) || (x % 2 == 1 && y % 2 == 1)) {
                mvaddch(y, x, '#');
            }
        }
    }
    display_refresh();
    getchar();
    display_clear();
    int flag = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (flag)
                mvaddch(y, x, '*');
            flag = !flag;
        }
    }
    display_refresh();
    getchar();

    display_deinit();

    return 0;
}
