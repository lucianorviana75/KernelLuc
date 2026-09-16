#include "vfs.h"
#include "heap.h"
#include "font.h"

fs_node_t* root_dir = 0;
fs_node_t* current_dir = 0;

static void my_strcpy(char* dest, const char* src) {
    int i = 0;
    while (src[i] != '\0' && i < MAX_NAME_LEN - 1) {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

void init_vfs(void) {
    root_dir = (fs_node_t*)kmalloc(sizeof(fs_node_t));
    my_strcpy(root_dir->name, "/");
    root_dir->type = FS_DIRECTORY;
    root_dir->parent = root_dir;
    root_dir->num_children = 0;
    
    current_dir = root_dir;
}

fs_node_t* create_directory(const char* name) {
    if (!current_dir || current_dir->num_children >= MAX_CHILDREN) return 0;

    fs_node_t* new_dir = (fs_node_t*)kmalloc(sizeof(fs_node_t));
    my_strcpy(new_dir->name, name);
    new_dir->type = FS_DIRECTORY;
    new_dir->parent = current_dir;
    new_dir->num_children = 0;

    current_dir->children[current_dir->num_children++] = new_dir;
    return new_dir;
}

void list_directory(fs_node_t* dir, int start_x, int start_y) {
    if (!dir) return;
    
    draw_string("Pastas no Kernel:", start_x, start_y, 0x0F); // Branco
    int offset_y = start_y + 12;

    for (uint32_t i = 0; i < dir->num_children; i++) {
        draw_string("[DIR] ", start_x, offset_y, 0x0E); // Amarelo
        draw_string(dir->children[i]->name, start_x + 48, offset_y, 0x0A); // Verde
        offset_y += 10;
    }

    if (dir->num_children == 0) {
        draw_string("(Diretorio Vazio)", start_x, offset_y, 0x08);
    }
}