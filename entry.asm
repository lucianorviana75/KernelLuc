[bits 32]
[extern kernel_main]

global _start
global idt_load
global no_interrupt_handler
global irq1_keyboard

extern keyboard_handler

_start:
    call kernel_main
.loop:
    hlt
    jmp .loop

; --- Rotinas de Interrupção IDT ---
idt_load:
    mov eax, [esp + 4]
    lidt [eax]
    ret

no_interrupt_handler:
    iret

irq1_keyboard:
    pusha
    call keyboard_handler
    mov al, 0x20
    out 0x20, al        ; Sinal de EOI para o PIC Master
    popa
    iret