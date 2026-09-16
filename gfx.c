#include "gfx.h"

static uint8_t* VGA = (uint8_t*)0xA0000;

void init_vga_mode13h(void) {
    gfx_clear(0);
}

void gfx_init(void) {
    gfx_clear(0);
}

void put_pixel(int x, int y, uint8_t color) {
    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
        VGA[y * SCREEN_WIDTH + x] = color;
    }
}

void gfx_clear(uint8_t color) {
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        VGA[i] = color;
    }
}

void gfx_flip(void) {
    // Sem operação necessária para escrita direta
}

void draw_line(int x0, int y0, int x1, int y1, uint8_t color) {
    int dx = (x1 > x0) ? (x1 - x0) : (x0 - x1);
    int sx = (x0 < x1) ? 1 : -1;
    int dy = (y1 > y0) ? (y0 - y1) : (y1 - y0);
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx + dy;

    while (1) {
        put_pixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}