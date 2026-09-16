CC = gcc
NASM = nasm
LD = ld

CFLAGS = -m32 -ffreestanding -fno-pie -c
LDFLAGS = -m elf_i386 -T linker.ld --oformat binary

OBJS = entry.o kernel.o heap.o obj_loader.o keyboard.o gfx.o font.o vfs.o window.o shell.o ata.o idt.o pic.o

all: boot_hd.bin os.img hd.img

bootloader.bin: boot.asm
	$(NASM) -f bin boot.asm -o bootloader.bin

boot_hd.bin: boot_hd.asm
	$(NASM) -f bin boot_hd.asm -o boot_hd.bin

entry.o: entry.asm
	$(NASM) -f elf32 entry.asm -o entry.o

kernel.o: kernel.c
	$(CC) $(CFLAGS) kernel.c -o kernel.o

heap.o: heap.c
	$(CC) $(CFLAGS) heap.c -o heap.o

obj_loader.o: obj_loader.c
	$(CC) $(CFLAGS) obj_loader.c -o obj_loader.o

keyboard.o: keyboard.c
	$(CC) $(CFLAGS) keyboard.c -o keyboard.o

gfx.o: gfx.c
	$(CC) $(CFLAGS) gfx.c -o gfx.o

font.o: font.c
	$(CC) $(CFLAGS) font.c -o font.o

vfs.o: vfs.c
	$(CC) $(CFLAGS) vfs.c -o vfs.o

window.o: window.c
	$(CC) $(CFLAGS) window.c -o window.o

shell.o: shell.c
	$(CC) $(CFLAGS) shell.c -o shell.o

ata.o: ata.c
	$(CC) $(CFLAGS) ata.c -o ata.o

idt.o: idt.c
	$(CC) $(CFLAGS) idt.c -o idt.o

pic.o: pic.c
	$(CC) $(CFLAGS) pic.c -o pic.o

kernel.bin: $(OBJS)
	$(LD) $(LDFLAGS) -o kernel.bin $(OBJS)

# Inclui o boot_hd.bin no final da imagem do disquete para o instalador poder ler!
os.img: bootloader.bin kernel.bin boot_hd.bin
	cat bootloader.bin kernel.bin boot_hd.bin > os.img
	truncate -s 1440k os.img

hd.img:
	qemu-img create -f raw hd.img 64M
	
run: os.img hd.img
	qemu-system-i386 -drive file=os.img,format=raw,index=0,if=floppy -drive file=hd.img,format=raw,index=0,if=ide

run-hd: boot_hd.bin hd.img
	dd if=boot_hd.bin of=hd.img conv=notrunc bs=512 count=1
	qemu-system-i386 -drive file=hd.img,format=raw,index=0,if=ide

clean:
	rm -f *.o *.bin os.img hd.img