#ifndef VFS_H
#define VFS_H

#include <stdint.h>

#define MAX_NAME_LEN 32
#define MAX_CHILDREN 16

typedef enum { FS_FILE, FS_DIRECTORY } fs_type_t;

typedef struct fs_node {
    char name[MAX_NAME_LEN];
    fs_type_t type;
    struct fs_node* parent;
    struct fs_node* children[MAX_CHILDREN];
    uint32_t num_children;
} fs_node_t;

void init_vfs(void);
fs_node_t* create_directory(const char* name);
void list_directory(fs_node_t* dir, int start_x, int start_y);

extern fs_node_t* current_dir;

#endif