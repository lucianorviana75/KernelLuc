#include "ata.h"

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ __volatile__ ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ __volatile__ ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline void outw(uint16_t port, uint16_t val) {
    __asm__ __volatile__ ("outw %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint16_t inw(uint16_t port) {
    uint16_t ret;
    __asm__ __volatile__ ("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void ata_write_sector(uint32_t lba, uint8_t *buffer) {
    outb(0x1F6, ((lba >> 24) & 0x0F) | 0xE0);
    outb(0x1F2, 1);                  // Quantidade de setores (1)
    outb(0x1F3, (uint8_t) lba);
    outb(0x1F4, (uint8_t)(lba >> 8));
    outb(0x1F5, (uint8_t)(lba >> 16));
    outb(0x1F7, 0x30);               // Comando 0x30 = Write Sectors

    // Aguarda o disco estar pronto para receber dados (BSY = 0)
    while (inb(0x1F7) & 0x80);

    // Envia os 512 bytes (256 words de 16 bits) para a porta de dados
    uint16_t *ptr = (uint16_t *)buffer;
    for (int i = 0; i < 256; i++) {
        outw(0x1F0, ptr[i]);
    }
}

void ata_read_sector(uint32_t lba, uint8_t *buffer) {
    outb(0x1F6, ((lba >> 24) & 0x0F) | 0xE0);
    outb(0x1F2, 1);
    outb(0x1F3, (uint8_t) lba);
    outb(0x1F4, (uint8_t)(lba >> 8));
    outb(0x1F5, (uint8_t)(lba >> 16));
    outb(0x1F7, 0x20);               // Comando 0x20 = Read Sectors

    while (inb(0x1F7) & 0x80);

    uint16_t *ptr = (uint16_t *)buffer;
    for (int i = 0; i < 256; i++) {
        ptr[i] = inw(0x1F0);
    }
}