bits 16
org 0x1000

mov si, msg
call print_string

jmp $

print_string:
    lodsb
    or al, al
    jz done
    mov ah, 0x0e
    int 0x10
    jmp print_string
done:
    ret

msg db 'Kernel de estudo rodando!',0
