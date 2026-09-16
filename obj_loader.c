#include "obj_loader.h"
#include "heap.h"

// Parse simples de string para inteiro (trata números como "1.0", "-0.5", etc.)
static int parse_coord(const char** str) {
    while (**str == ' ' || **str == '\t') (*str)++;
    
    int sign = 1;
    if (**str == '-') {
        sign = -1;
        (*str)++;
    }

    int int_part = 0;
    while (**str >= '0' && **str <= '9') {
        int_part = int_part * 10 + (**str - '0');
        (*str)++;
    }

    int frac_part = 0;
    if (**str == '.') {
        (*str)++;
        if (**str >= '0' && **str <= '9') {
            frac_part = (**str - '0') * 10; // Pega o primeiro dígito decimal
            (*str)++;
        }
        while (**str >= '0' && **str <= '9') (*str)++; // Pula o restante
    }

    while (**str != ' ' && **str != '\n' && **str != '\0') (*str)++;

    return sign * (int_part * 100 + frac_part);
}

static int parse_int(const char** str) {
    while (**str == ' ' || **str == '\t') (*str)++;
    int val = 0;
    while (**str >= '0' && **str <= '9') {
        val = val * 10 + (**str - '0');
        (*str)++;
    }
    while (**str != ' ' && **str != '\n' && **str != '\0') (*str)++;
    return val;
}

mesh_t* parse_obj(const char* obj_data) {
    mesh_t* mesh = (mesh_t*)kmalloc(sizeof(mesh_t));
    if (!mesh) return 0;

    mesh->num_vertices = 0;
    mesh->num_faces = 0;

    const char* ptr = obj_data;

    // Contagem de vértices e faces
    while (*ptr != '\0') {
        if (ptr[0] == 'v' && ptr[1] == ' ') mesh->num_vertices++;
        else if (ptr[0] == 'f' && ptr[1] == ' ') mesh->num_faces++;
        
        while (*ptr != '\n' && *ptr != '\0') ptr++;
        if (*ptr == '\n') ptr++;
    }

    mesh->vertices = (vec3_t*)kmalloc(sizeof(vec3_t) * mesh->num_vertices);
    mesh->faces = (face_t*)kmalloc(sizeof(face_t) * mesh->num_faces);

    ptr = obj_data;
    uint32_t v_idx = 0;
    uint32_t f_idx = 0;

    // Leitura dos dados
    while (*ptr != '\0') {
        if (ptr[0] == 'v' && ptr[1] == ' ') {
            ptr += 2;
            mesh->vertices[v_idx].x = (float)parse_coord(&ptr) / 100.0f;
            mesh->vertices[v_idx].y = (float)parse_coord(&ptr) / 100.0f;
            mesh->vertices[v_idx].z = (float)parse_coord(&ptr) / 100.0f;
            v_idx++;
        } else if (ptr[0] == 'f' && ptr[1] == ' ') {
            ptr += 2;
            mesh->faces[f_idx].v1 = parse_int(&ptr);
            mesh->faces[f_idx].v2 = parse_int(&ptr);
            mesh->faces[f_idx].v3 = parse_int(&ptr);
            f_idx++;
        }

        while (*ptr != '\n' && *ptr != '\0') ptr++;
        if (*ptr == '\n') ptr++;
    }

    return mesh;
}