#include "window.h"
#include "gfx.h"
#include "font.h"

void draw_window(window_t* win) {
    if (!win || !win->active) return;

    int x = win->x;
    int y = win->y;
    int w = win->width;
    int h = win->height;

    // 1. Fundo/Corpo da janela
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            put_pixel(x + i, y + j, win->color_bg);
        }
    }

    // 2. Barra de título (Sempre no topo da janela, altura 12px, cor azul escuro 0x01)
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < 12; j++) {
            put_pixel(x + i, y + j, 0x01);
        }
    }

    // 3. Moldura/Borda da janela (Linha cinza claro 0x0F em volta)
    for (int i = 0; i < w; i++) {
        put_pixel(x + i, y, 0x0F);
        put_pixel(x + i, y + h - 1, 0x0F);
    }
    for (int j = 0; j < h; j++) {
        put_pixel(x, y + j, 0x0F);
        put_pixel(x + w - 1, y + j, 0x0F);
    }

    // 4. Escreve o Título na barra (cor branca 0x0F)
    draw_string(win->title, x + 4, y + 2, 0x0F);
}