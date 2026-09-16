#ifndef FONT_H
#define FONT_H

#include <stdint.h>

extern const uint8_t font8x8_basic[128][8];

void draw_char(char c, int x, int y, uint8_t color);
void draw_string(const char* str, int x, int y, uint8_t color);

#endif