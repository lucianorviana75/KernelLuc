#ifndef HEAP_H
#define HEAP_H

#include <stdint.h>

#define HEAP_START 0x00200000 // Início do Heap em 2 MB
#define HEAP_SIZE  0x00800000 // Tamanho total do Heap (8 MB)

typedef struct block_header {
    uint32_t size;               // Tamanho útil de dados do bloco
    uint8_t is_free;             // 1 = Livre, 0 = Ocupado
    struct block_header* next;   // Próximo bloco
} block_header_t;

void init_heap(void);
void* kmalloc(uint32_t size);
void kfree(void* ptr);

#endif