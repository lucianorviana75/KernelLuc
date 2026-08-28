bits 32

section .multiboot
    align 4
    dd 0x1BADB002              ; Magic Number Multiboot
    dd 0x00                    ; Flags
    dd - (0x1BADB002 + 0x00)   ; Checksum

section .bss
align 16
stack_bottom:
resb 16384                     ; 16 KB de pilha
stack_top:

section .data
align 8
; Tabela de Descritores Globais (GDT)
gdt_start:
    dd 0x0, 0x0                ; Descriptor Nulo (0x00)
gdt_code:                      ; Descriptor de Codigo (0x08)
    dw 0xFFFF, 0x0000
    db 0x00, 0x9A, 0xCF, 0x00
gdt_data:                      ; Descriptor de Dados (0x10)
    dw 0xFFFF, 0x0000
    db 0x00, 0x92, 0xCF, 0x00
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

section .text
global _start
global idt_load
global no_interrupt_handler
global irq1_keyboard
extern kernel_main
extern keyboard_handler

_start:
    cli                        ; Desativa interrupcoes

    ; 1. Carrega nossa propria GDT
    lgdt [gdt_descriptor]

    ; 2. Recarrega os registradores de segmento de dados (0x10)
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, stack_top

    ; 3. Far Jump para recarregar o registrador CS (0x08)
    jmp 0x08:reload_cs

reload_cs:
    call kernel_main

.loop:
    hlt
    jmp .loop

idt_load:
    mov eax, [esp + 4]
    lidt [eax]
    ret

no_interrupt_handler:
    iret

irq1_keyboard:
    pusha
    call keyboard_handler
    popa
    iret