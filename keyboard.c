#include "io.h"
#include <stdint.h>
#include <stddef.h>

extern void terminal_putchar(char c);
extern void kputs(const char* str);
extern void terminal_initialize(void);

#define BUFFER_SIZE 256
char input_buffer[BUFFER_SIZE];
size_t buffer_index = 0;

// Estado das teclas modificadoras
int shift_pressed = 0;

/* Tabela Normal (Sem Shift) - Layout ABNT2 com 'ç' (135 em CP437) */
const char keyboard_map_normal[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
  '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',  0,  '[', '\n',
     0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 135, '~', '\'',  0,
   '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', ';',  0, '*',   0, ' '
};

/* Tabela Modificada (Com Shift) - Layout ABNT2 com 'Ç' (128 em CP437) */
const char keyboard_map_shift[128] = {
    0,  27, '!', '@', '#', '$', '%', '"', '&', '*', '(', ')', '_', '+', '\b',
  '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',  0,  '{', '\n',
     0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 128, '^', '`',   0,
    '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', ':',  0, '*',   0, ' '
};

int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}

void process_command(void) {
    input_buffer[buffer_index] = '\0';

    if (buffer_index == 0) {
        kputs("\nKernioLuc> ");
        return;
    }

    kputs("\n");

    if (strcmp(input_buffer, "help") == 0) {
        kputs("Comandos disponiveis: help, clear, about\n");
    } else if (strcmp(input_buffer, "clear") == 0) {
        terminal_initialize();
    } else if (strcmp(input_buffer, "about") == 0) {
        kputs("KernioLuc OS v0.1 - Kernel x86 em Modo Protegido 32-bit\n");
    } else {
        kputs("Comando nao reconhecido: ");
        kputs(input_buffer);
        kputs("\n");
    }

    buffer_index = 0;
    kputs("KernioLuc> ");
}

int pending_tilde = 0;

void keyboard_handler(void) {
    uint8_t scancode = inb(0x60);

    if (scancode == 0x2A || scancode == 0x36) {
        shift_pressed = 1;
    } else if (scancode == 0xAA || scancode == 0xB6) {
        shift_pressed = 0;
    } else if (!(scancode & 0x80)) {
        if (scancode < 128) {
            char letter = shift_pressed ? keyboard_map_shift[scancode] : keyboard_map_normal[scancode];

            // Trata o acento til pendente
            if (pending_tilde) {
                pending_tilde = 0;
                
                // Imprime primeiro o til e salva no buffer
                if (buffer_index < BUFFER_SIZE - 1) {
                    input_buffer[buffer_index++] = '~';
                    terminal_putchar('~');
                }
                
                // Em seguida, o código continuará para imprimir a letra normal ('a', 'o', etc.)
            } else if (letter == '~') {
                pending_tilde = 1;
                outb(0x20, 0x20);
                return;
            }

            if (letter == '\n') {
                process_command();
            } else if (letter == '\b') {
                if (buffer_index > 0) {
                    buffer_index--;
                    terminal_putchar('\b');
                }
            } else if (letter != 0 && buffer_index < BUFFER_SIZE - 1) {
                input_buffer[buffer_index++] = letter;
                terminal_putchar(letter);
            }
        }
    }

    outb(0x20, 0x20); // Envia EOI ao PIC
}