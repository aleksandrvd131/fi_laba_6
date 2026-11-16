#pragma once
#include "client.h"

// Динамический массив (вектор) клиентов
typedef struct {
    int capacity;
    int size;
    client *ptr;
} vec_cl;

vec_cl *vec_cl_init(int size);
int vec_cl_destroy(vec_cl *vec);
int vec_cl_set_size(vec_cl *vec, int size);
int *vec_cl_get_size(vec_cl *vec);
int vec_cl_set(vec_cl *vec, int index, client value);
client *vec_cl_get(vec_cl *vec, int index);
int vec_cl_push(vec_cl *vec, client value);
