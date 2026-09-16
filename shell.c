#include "shell.h"
#include "gfx.h"
#include "font.h"
#include "vfs.h"

static char command_buffer[64];
static int buffer_idx = 0;
static char last_output[64] = "Kernel Shell inicializado. Digite 'help'";

window_t app_window = {
    .x = 100,
    .y = 50,
    .width = 180,
    .height = 100,
    .title = "Minha Janela",
    .color_bg = 0x07, // Cinza
    .active = 0       // Começa fechada
};

// Comparador de strings simples para o Kernel
static int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

// Copiador de strings simples
static void strcpy(char* dest, const char* src) {
    while (*src) {
        *dest++ = *src++;
    }
    *dest = '\0';
}

void shell_init(void) {
    buffer_idx = 0;
    command_buffer[0] = '\0';
}

void shell_input_key(char c) {
    if (c == '\n' || c == '\r') {
        shell_execute();
    } else if (c == '\b') { // Backspace
        if (buffer_idx > 0) {
            buffer_idx--;
            command_buffer[buffer_idx] = '\0';
        }
    } else if (buffer_idx < 60 && c >= ' ') {
        command_buffer[buffer_idx++] = c;
        command_buffer[buffer_idx] = '\0';
    }
}

void shell_execute(void) {
    if (buffer_idx == 0) return;

    if (strcmp(command_buffer, "help") == 0) {
        strcpy(last_output, "Cmds: help, win, close, clear, reboot");
    } 
    else if (strcmp(command_buffer, "win") == 0) {
        app_window.active = 1;
        strcpy(last_output, "Janela aberta!");
    } 
    else if (strcmp(command_buffer, "close") == 0) {
        app_window.active = 0;
        strcpy(last_output, "Janela fechada!");
    } 
    else if (strcmp(command_buffer, "clear") == 0) {
        last_output[0] = '\0';
    } 
    else if (strcmp(command_buffer, "reboot") == 0) {
        // Envia comando de reset para o controlador 8042 do teclado
        uint8_t good = 0x02;
        while (good & 0x02) {
            __asm__ __volatile__("inb $0x64, %0" : "=a"(good));
        }
        __asm__ __volatile__("outb %0, $0x64" : : "a"((uint8_t)0xFE));
    } 
    else {
        strcpy(last_output, "Comando desconhecido!");
    }

    // Limpa o buffer de entrada
    buffer_idx = 0;
    command_buffer[0] = '\0';
}

void shell_draw(void) {
    // Desenha o prompt do shell na parte inferior do ecran
    draw_string("KernelLuc>", 10, 160, 0x0A); // Verde
    draw_string(command_buffer, 90, 160, 0x0F); // Texto digitado (Branco)
    
    // Desenha a resposta do último comando executado
    draw_string(last_output, 10, 175, 0x0E); // Amarelo

    // Desenha a janela se estiver ativa
    if (app_window.active) {
        draw_window(&app_window);
        // Escreve um conteúdo dentro da janela
        draw_string("Janela Ativa!", app_window.x + 10, app_window.y + 30, 0x00);
    }
}