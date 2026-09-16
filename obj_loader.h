#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include <stdint.h>

typedef struct {
    float x, y, z;
} vec3_t;

typedef struct {
    int v1, v2, v3;
} face_t;

typedef struct {
    vec3_t* vertices;
    int num_vertices;
    face_t* faces;
    int num_faces;
} mesh_t;

mesh_t* parse_obj(const char* obj_data);

#endif