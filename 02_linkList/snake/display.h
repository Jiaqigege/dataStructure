#ifndef DISPLAY_H
#define DISPLAY_H

#include <ncurses.h>
#include <stdint.h>

void display_init(void);
void display_deinit(void);
void display_refresh(void);
void display_clear(void);
void display_point(int x, int y, uint8_t point);

#endif // DISPLAY_H
