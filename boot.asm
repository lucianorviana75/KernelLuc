[bits 16]
[org 0x7c00]

start:
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00

    mov [boot_drive], dl

    ; 1. Activa Modo Gráfico VGA Modo 13h (320x200, 256 cores)
    mov ax, 0x0013
    int 0x10

    ; 2. Prepara o endereço de destino (0x1000:0x0000 = 0x10000)
    mov ax, 0x1000
    mov es, ax
    
    mov cl, 2          ; Setor 2
    mov ch, 0          ; Trilha 0
    mov dh, 0          ; Cabeça 0
    mov di, 120         ; Aumentado para 120 setores (~40KB de Kernel)

read_kernel_loop:
    xor bx, bx
    mov ah, 0x02
    mov al, 1
    mov dl, [boot_drive]
    int 0x13
    jc read_fail

    mov ax, es
    add ax, 0x0020
    mov es, ax

    inc cl
    cmp cl, 19
    jne continue_loop
    
    mov cl, 1
    inc dh
    cmp dh, 2
    jne continue_loop

    mov dh, 0
    inc ch

continue_loop:
    dec di
    jnz read_kernel_loop

    ; 3. Salto para Modo Protegido 32-bit
    cli
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp 0x08:init_pm

read_fail:
    mov ax, 0x0013
    int 0x10
    mov ah, 0x0b
    mov bh, 0x00
    mov bl, 0x04
    int 0x10
.halt:
    hlt
    jmp .halt

[bits 32]
init_pm:
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov esp, 0x90000

    jmp 0x10000

boot_drive db 0

gdt_start:
    dq 0x0
gdt_code:
    dw 0xffff, 0x0000, 0x9a00, 0x00cf
gdt_data:
    dw 0xffff, 0x0000, 0x9200, 0x00cf
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

times 510-($-$$) db 0
dw 0xaa55