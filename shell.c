#include "shell.h"
#include "gfx.h"
#include "font.h"
#include "vfs.h"
#include "ata.h"

static char command_buffer[64];
static int buffer_idx = 0;
static char last_output[64] = "Kernel Shell inicializado. Digite 'help'";

window_t app_window = {
    .x = 100,
    .y = 50,
    .width = 180,
    .height = 100,
    .title = "Minha Janela",
    .color_bg = 0x07, // Cinza
    .active = 0       // Começa fechada
};

static int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

static void strcpy(char* dest, const char* src) {
    while (*src) {
        *dest++ = *src++;
    }
    *dest = '\0';
}

void shell_init(void) {
    buffer_idx = 0;
    command_buffer[0] = '\0';
}

void shell_input_key(char c) {
    if (c == '\n' || c == '\r') {
        shell_execute();
    } else if (c == '\b') {
        if (buffer_idx > 0) {
            buffer_idx--;
            command_buffer[buffer_idx] = '\0';
        }
    } else if (buffer_idx < 60 && c >= ' ') {
        command_buffer[buffer_idx++] = c;
        command_buffer[buffer_idx] = '\0';
    }
}

void shell_execute(void) {
    if (buffer_idx == 0) return;

    if (strcmp(command_buffer, "help") == 0) {
        strcpy(last_output, "Cmds: help, win, close, install, clear, reboot");
    } 
    else if (strcmp(command_buffer, "win") == 0) {
        app_window.active = 1;
        strcpy(last_output, "Janela aberta!");
    } 
    else if (strcmp(command_buffer, "close") == 0) {
        app_window.active = 0;
        strcpy(last_output, "Janela fechada!");
    } 
    else if (strcmp(command_buffer, "install") == 0) {
        // O boot_hd.bin está carregado no setor 121 (LBA 121 = offset 120 * 512 da memória RAM)
        //uint8_t *boot_hd_ram = (uint8_t *)(0x10000 + (120 * 512));

        // 1. Grava o Bootloader no MBR (Setor 0)
        //ata_write_sector(0, boot_hd_ram);

        // 2. Grava a imagem do Kernel nos setores 1 a 120 do HD
        uint8_t *kernel_ram = (uint8_t *)0x10000;
        for (uint32_t sector = 1; sector <= 120; sector++) {
            ata_write_sector(sector, kernel_ram + ((sector - 1) * 512));
            for (volatile int i = 0; i < 5000; i++); // Pequeno delay I/O
        }

        strcpy(last_output, "Instalacao concluida! MBR e Kernel no HD.");
    }
    else if (strcmp(command_buffer, "clear") == 0) {
        last_output[0] = '\0';
    } 
    else if (strcmp(command_buffer, "reboot") == 0) {
        uint8_t good = 0x02;
        while (good & 0x02) {
            __asm__ __volatile__("inb $0x64, %0" : "=a"(good));
        }
        __asm__ __volatile__("outb %0, $0x64" : : "a"((uint8_t)0xFE));
    } 
    else {
        strcpy(last_output, "Comando desconhecido!");
    }

    buffer_idx = 0;
    command_buffer[0] = '\0';
}

void shell_draw(void) {
    draw_string("KernelLuc>", 10, 160, 0x0A);
    draw_string(command_buffer, 90, 160, 0x0F);
    draw_string(last_output, 10, 175, 0x0E);

    if (app_window.active) {
        draw_window(&app_window);
        draw_string("Janela Ativa!", app_window.x + 10, app_window.y + 30, 0x00);
    }
}