[bits 32]
[extern kernel_main]

global _start
_start:
    ; Configura os registradores de segmento para Modo Protegido (0x10 = Segmento de Dados)
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Configura a Pilha (Stack) em local seguro da RAM
    mov esp, 0x90000

    ; Chama a função C do kernel
    call kernel_main

.loop:
    cli
    hlt
    jmp .loop