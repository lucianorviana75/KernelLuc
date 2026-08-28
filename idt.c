#include "idt.h"

struct idt_entry idt[256];
struct idt_ptr idtp;

extern void idt_load(uint32_t);
extern void no_interrupt_handler(void);
extern void irq1_keyboard(void);
extern void pic_remap(void);

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low = (base & 0xFFFF);
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

void idt_init(void) {
    idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
    idtp.base = (uint32_t)&idt;

    // Preenche com o seletor 0x08 garantido pela nova GDT
    for (int i = 0; i < 256; i++) {
        idt_set_gate(i, (uint32_t)no_interrupt_handler, 0x08, 0x8E);
    }

    pic_remap();
    idt_set_gate(33, (uint32_t)irq1_keyboard, 0x08, 0x8E);
    idt_load((uint32_t)&idtp);

    __asm__ __volatile__("sti");
}