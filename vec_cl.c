#include <stdlib.h>
#include "vec_cl.h"
#include "vec_tr.h"

// Создаем вектор клиентов
vec_cl *vec_cl_init(int size) {
    if (size < 0) return NULL;
    vec_cl *v = malloc(sizeof(vec_cl));
    if (!v) return NULL;
    v->size = size;
    v->capacity = size > 0 ? size : 4;
    v->ptr = calloc(v->capacity, sizeof(client));
    if (!v->ptr) {
        free(v);
        return NULL;
    }
    return v;
}

// Освобождаем вектор и транзакции каждого клиента
int vec_cl_destroy(vec_cl *vec) {
    if (!vec) return -1;
    if (vec->ptr) {
        for (int i = 0; i < vec->size; i++) {
            if (vec->ptr[i].transactions)
                vec_tr_destroy(vec->ptr[i].transactions);
        }
        free(vec->ptr);
    }
    free(vec);
    return 0;
}

// Меняем размер вектора клиентов
int vec_cl_set_size(vec_cl *vec, int size) {
    if (!vec || size < 0) return -1;
    // если уменьшаем размер, удаляем хвост транзакций
    for (int i = size; i < vec->size; i++) {
        if (vec->ptr[i].transactions) {
            vec_tr_destroy(vec->ptr[i].transactions);
            vec->ptr[i].transactions = NULL;
        }
    }
    if (size > vec->capacity) {
        int new_cap = size * 2;
        client *p = realloc(vec->ptr, sizeof(client) * new_cap);
        if (!p) return -1;
        vec->ptr = p;
        for (int i = vec->capacity; i < new_cap; i++)
            vec->ptr[i].transactions = NULL;
        vec->capacity = new_cap;
    }
    vec->size = size;
    return 0;
}

// Возвращаем указатель на размер
int *vec_cl_get_size(vec_cl *vec) {
    return vec ? &vec->size : NULL;
}

// Запись клиента по индексу
int vec_cl_set(vec_cl *vec, int index, client value) {
    if (!vec || !vec->ptr || index < 0 || index >= vec->size) return -1;
    vec->ptr[index] = value;
    return 0;
}

// Получение клиента по индексу
client *vec_cl_get(vec_cl *vec, int index) {
    if (!vec || !vec->ptr || index < 0 || index >= vec->size) return NULL;
    return &vec->ptr[index];
}

// Добавление клиента в конец
int vec_cl_push(vec_cl *vec, client value) {
    if (!vec) return -1;
    int cur = vec->size;
    if (vec_cl_set_size(vec, cur + 1) != 0) return -1;
    vec->ptr[cur] = value;
    return 0;
}
