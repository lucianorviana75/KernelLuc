#include "io.h"

#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1

void pic_remap(void) {
    // Inicialização do PIC em modo cascata (ICW1)
    outb(PIC1_COMMAND, 0x11);
    outb(PIC2_COMMAND, 0x11);

    // ICW2: Recompensa os vetores de interrupção
    // IRQ 0..7 remapeadas para interrupções 32..39
    outb(PIC1_DATA, 0x20);
    // IRQ 8..15 remapeadas para interrupções 40..47
    outb(PIC2_DATA, 0x28);

    // ICW3: Comunicação Mestre/Escravo
    outb(PIC1_DATA, 0x04);
    outb(PIC2_DATA, 0x02);

    // ICW4: Modo 8086
    outb(PIC1_DATA, 0x01);
    outb(PIC2_DATA, 0x01);

    // Máscaras de interrupção (ICW1/OCW1): 
    // 0xfd (1111 1101) desmascara APENAS a IRQ1 (Teclado) e bloqueia o Timer/outros
    outb(PIC1_DATA, 0xFD);
    outb(PIC2_DATA, 0xFF);
}