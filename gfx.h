#ifndef GFX_H
#define GFX_H

#include <stdint.h>
#include "obj_loader.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

void init_vga_mode13h(void);
void gfx_init(void);
void put_pixel(int x, int y, uint8_t color);
void gfx_clear(uint8_t color);
void gfx_flip(void);
void draw_line(int x0, int y0, int x1, int y1, uint8_t color);
void draw_mesh_wireframe(mesh_t* mesh, int center_x, int center_y, uint8_t color);

#endif