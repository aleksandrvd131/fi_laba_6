#pragma once
#include "models.h"

// Динамический массив (вектор) транзакций
typedef struct {
    int capacity;
    int size;
    transaction *ptr;
} vec_tr;

vec_tr *vec_tr_init(int size);
int vec_tr_destroy(vec_tr *vec);
int vec_tr_set_size(vec_tr *vec, int size);
int *vec_tr_get_size(vec_tr *vec);
int vec_tr_set(vec_tr *vec, int index, transaction value);
transaction *vec_tr_get(vec_tr *vec, int index);
int vec_tr_push(vec_tr *vec, transaction value);
