#include "keyboard.h"

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ __volatile__ ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ __volatile__ ("outb %0, %1" : : "a"(val), "Nd"(port));
}

void init_keyboard(void) {
    // Inicialização reservada
}

void keyboard_handler(void) {
    // Handler reservado para IDT
}

char get_last_key(void) {
    if (inb(0x64) & 0x01) {
        uint8_t scancode = inb(0x60);
        
        // Apenas pressões de tecla (Scancodes < 0x80)
        if (!(scancode & 0x80)) {
            switch (scancode) {
                // Números
                case 0x02: return '1';
                case 0x03: return '2';
                case 0x04: return '3';
                case 0x05: return '4';
                case 0x06: return '5';
                case 0x07: return '6';
                case 0x08: return '7';
                case 0x09: return '8';
                case 0x0A: return '9';
                case 0x0B: return '0';

                // Primeira Linha de Letras (QWERTYUIOP)
                case 0x10: return 'q';
                case 0x11: return 'w';
                case 0x12: return 'e';
                case 0x13: return 'r';
                case 0x14: return 't';
                case 0x15: return 'y';
                case 0x16: return 'u';
                case 0x17: return 'i';
                case 0x18: return 'o';
                case 0x19: return 'p'; // <-- Adicionada a tecla P!

                // Segunda Linha de Letras (ASDFGHJKL)
                case 0x1E: return 'a';
                case 0x1F: return 's';
                case 0x20: return 'd';
                case 0x21: return 'f';
                case 0x22: return 'g';
                case 0x23: return 'h';
                case 0x24: return 'j';
                case 0x25: return 'k';
                case 0x26: return 'l';

                // Terceira Linha de Letras (ZXCVBNM)
                case 0x2C: return 'z';
                case 0x2D: return 'x';
                case 0x2E: return 'c';
                case 0x2F: return 'v';
                case 0x30: return 'b';
                case 0x31: return 'n';
                case 0x32: return 'm';

                // Teclas de Controle e Símbolos
                case 0x1C: return '\n'; // Enter
                case 0x0E: return '\b'; // Backspace
                case 0x39: return ' ';  // Espaço
                case 0x34: return '.';  // Ponto
                case 0x35: return '/';  // Barra

                default: break;
            }
        }
    }
    return 0;
}