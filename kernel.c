#include <stddef.h>
#include <stdint.h>
#include "idt.h"
#include "io.h" // Necessario para usar a funcao outb

/* 1. Endereço base e dimensões da memória VGA */
volatile uint16_t* const VGA_MEMORY = (volatile uint16_t*) 0xB8000;
const size_t VGA_WIDTH = 80;
const size_t VGA_HEIGHT = 25;

/* 2. Cores do modo VGA */
enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

size_t terminal_row = 0;
size_t terminal_column = 0;
uint8_t terminal_color = 0;

/* 3. Funções Utilitárias para VGA */
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t) uc | (uint16_t) color << 8;
}

/* Atualiza a posição do cursor de hardware da GPU */
void update_cursor(size_t x, size_t y) {
    uint16_t pos = y * VGA_WIDTH + x;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t) (pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void terminal_initialize(void) {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);

    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            VGA_MEMORY[index] = vga_entry(' ', terminal_color);
        }
    }
    
    update_cursor(terminal_column, terminal_row); // Garante cursor em (0,0) na inicialização
}

void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT) {
            terminal_row = 0;
        }
        update_cursor(terminal_column, terminal_row);
        return;
    }

    // Tratamento correto do Backspace no terminal
    if (c == '\b') {
        if (terminal_column > 0) {
            terminal_column--;
            const size_t index = terminal_row * VGA_WIDTH + terminal_column;
            VGA_MEMORY[index] = vga_entry(' ', terminal_color); // Apaga visualmente na tela
        }
        update_cursor(terminal_column, terminal_row);
        return;
    }

    const size_t index = terminal_row * VGA_WIDTH + terminal_column;
    VGA_MEMORY[index] = vga_entry(c, terminal_color);

    if (++terminal_column == VGA_WIDTH) {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT) {
            terminal_row = 0;
        }
    }

    update_cursor(terminal_column, terminal_row); // Sincroniza o cursor apos cada caractere
}

void kputs(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        terminal_putchar(str[i]);
    }
}

/* 4. Função Principal (Entrada do Kernel - Fica no final) */
void kernel_main(void) {
    terminal_initialize();

    // Inicializa a Tabela de Interrupções
    idt_init();

    terminal_color = vga_entry_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
    kputs("[ OK ] KernioLuc Kernel iniciado com sucesso!\n");

    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
    kputs("[ OK ] IDT carregada com sucesso!\n");
}