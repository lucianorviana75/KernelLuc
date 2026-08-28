#ifndef IDT_H
#define IDT_H

#include <stdint.h>

/* Estrutura de uma entrada da IDT (8 bytes) */
struct idt_entry {
    uint16_t base_low;   // Endereço de memória baixo da função de tratamento (bits 0-15)
    uint16_t sel;        // Seletor de segmento de código (GDT)
    uint8_t  always0;    // Sempre zero (reservado pela Intel)
    uint8_t  flags;      // Tipo de porta e permissões (Privilégio do Kernel vs Usuário)
    uint16_t base_high;  // Endereço de memória alto da função de tratamento (bits 16-31)
} __attribute__((packed));

/* Ponteiro da IDT passado para a instrução 'lidt' */
struct idt_ptr {
    uint16_t limit;      // Tamanho total da IDT - 1
    uint32_t base;       // Endereço base onde a IDT está localizada na RAM
} __attribute__((packed));

/* Funções públicas */
void idt_init(void);

#endif