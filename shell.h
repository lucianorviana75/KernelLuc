#ifndef SHELL_H
#define SHELL_H

#include <stdint.h>
#include "window.h"

void shell_init(void);
void shell_input_key(char c);
void shell_execute(void);
void shell_draw(void);

// Acesso à janela global criada pelo shell
extern window_t app_window;

#endif