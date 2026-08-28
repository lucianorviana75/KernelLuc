# Variáveis de ferramentas
NASM = nasm
CC = gcc
LD = ld
CFLAGS = -m32 -c -std=gnu99 -ffreestanding -O2 -Wall -Wextra
LDFLAGS = -m elf_i386 -T linker.ld

# Objetos que serão gerados
OBJS = boot.o kernel.o idt.o pic.o keyboard.o

# Regra principal (gera a ISO)
all: kernioluc.iso

boot.o: boot.asm
	$(NASM) -f elf32 boot.asm -o boot.o

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

mykernel.bin: $(OBJS)
	$(LD) $(LDFLAGS) -o mykernel.bin $(OBJS)

kernioluc.iso: mykernel.bin
	cp mykernel.bin iso/boot/mykernel.bin
	grub-mkrescue -o kernioluc.iso iso

# Roda o kernel direto no QEMU
run: kernioluc.iso
	qemu-system-i386 -cdrom kernioluc.iso

# Limpa a pasta do projeto
clean:
	rm -f *.o mykernel.bin kernioluc.iso iso/boot/mykernel.bin