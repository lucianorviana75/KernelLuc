CC = gcc
CFLAGS = -m32 -ffreestanding -fno-stack-protector -nostdlib -fno-pie -fno-pic
ASM = nasm
LD = ld

# Target padrão: gera a imagem completa do sistema
all: os.img

# 1. Bootloader de 16-bit (Gera Flat Binary bruto de 512 bytes)
boot.bin: boot.asm
	$(ASM) -f bin boot.asm -o boot.bin

# 2. Ponto de Entrada do Kernel em 32-bit (Gera objeto ELF32)
entry.o: entry.asm
	$(ASM) -f elf32 entry.asm -o entry.o

# 3. Módulos em C
kernel.o: kernel.c
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o

heap.o: heap.c
	$(CC) $(CFLAGS) -c heap.c -o heap.o

obj_loader.o: obj_loader.c
	$(CC) $(CFLAGS) -c obj_loader.c -o obj_loader.o

keyboard.o: keyboard.c
	$(CC) $(CFLAGS) -c keyboard.c -o keyboard.o

gfx.o: gfx.c
	$(CC) $(CFLAGS) -c gfx.c -o gfx.o

font.o: font.c
	$(CC) $(CFLAGS) -c font.c -o font.o

vfs.o: vfs.c
	$(CC) $(CFLAGS) -c vfs.c -o vfs.o

window.o: window.c
	$(CC) $(CFLAGS) -c window.c -o window.o

shell.o: shell.c
	$(CC) $(CFLAGS) -c shell.c -o shell.o

# 4. Linking do Kernel Completo
kernel.bin: entry.o kernel.o heap.o obj_loader.o keyboard.o gfx.o font.o vfs.o window.o shell.o
	$(LD) -m elf_i386 -T linker.ld --oformat binary -o kernel.bin entry.o kernel.o heap.o obj_loader.o keyboard.o gfx.o font.o vfs.o window.o shell.o

# 5. Criação da Imagem de Disquete de 1.44MB (boot.bin no setor 0, kernel.bin no setor 1)
os.img: boot.bin kernel.bin
	dd if=/dev/zero of=os.img bs=512 count=2880 status=none
	dd if=boot.bin of=os.img conv=notrunc status=none
	dd if=kernel.bin of=os.img bs=512 seek=1 conv=notrunc status=none

# Executar no QEMU
run: os.img
	qemu-system-i386 -drive format=raw,file=os.img,index=0,if=floppy

# Limpeza
clean:
	rm -f *.o boot.bin kernel.bin os.img