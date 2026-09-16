#include "heap.h"

static block_header_t* heap_first = (block_header_t*)HEAP_START;

void init_heap(void) {
    heap_first->size = HEAP_SIZE - sizeof(block_header_t);
    heap_first->is_free = 1;
    heap_first->next = 0;
}

void* kmalloc(uint32_t size) {
    if (size % 4 != 0) {
        size += 4 - (size % 4);
    }

    block_header_t* curr = heap_first;

    while (curr != 0) {
        if (curr->is_free && curr->size >= size) {
            
            if (curr->size >= size + sizeof(block_header_t) + 16) {
                block_header_t* next_block = (block_header_t*)((uint32_t)curr + sizeof(block_header_t) + size);
                next_block->size = curr->size - size - sizeof(block_header_t);
                next_block->is_free = 1;
                next_block->next = curr->next;

                curr->size = size;
                curr->next = next_block;
            }

            curr->is_free = 0;
            return (void*)((uint32_t)curr + sizeof(block_header_t));
        }
        curr = curr->next;
    }

    return 0; // Out of memory
}

void kfree(void* ptr) {
    if (!ptr) return;

    block_header_t* header = (block_header_t*)((uint32_t)ptr - sizeof(block_header_t));
    header->is_free = 1;

    block_header_t* curr = heap_first;
    while (curr != 0 && curr->next != 0) {
        if (curr->is_free && curr->next->is_free) {
            curr->size += sizeof(block_header_t) + curr->next->size;
            curr->next = curr->next->next;
        } else {
            curr = curr->next;
        }
    }
}