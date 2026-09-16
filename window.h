#ifndef WINDOW_H
#define WINDOW_H

#include <stdint.h>

typedef struct {
    int x, y;           // Posição no ecrã (320x200)
    int width, height;  // Dimensões da janela
    char title[32];     // Título da janela
    uint8_t color_bg;   // Cor do fundo da janela
    uint8_t active;     // 1 = visível, 0 = oculta
} window_t;

void draw_window(window_t* win);

#endif