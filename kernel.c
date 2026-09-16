#include "font.h"
#include "gfx.h"
#include "keyboard.h"
#include "shell.h"

void kernel_main(void) {
    // 1. Limpa a tela para preto
    gfx_clear(0x00);

    // 2. Desenha a interface inicial antes de ativar o teclado
    draw_string("KernioLuc OS v0.1 - Terminal", 10, 10, 0x0F);
    draw_string("----------------------------", 10, 20, 0x08);
    
    shell_init();
    shell_draw();

    // 3. Loop principal com debounce seguro
    while (1) {
        char k = get_last_key();
        if (k != 0) {
            shell_input_key(k);
            gfx_clear(0x00);
            draw_string("KernioLuc OS v0.1 - Terminal", 10, 10, 0x0F);
            draw_string("----------------------------", 10, 20, 0x08);
            shell_draw();
        }
    }
}