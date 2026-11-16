#include <stdlib.h>
#include "vec_tr.h"

// Создаем вектор с заданной начальной вместимостью
vec_tr *vec_tr_init(int size) {
    if (size < 0) return NULL;
    vec_tr *v = malloc(sizeof(vec_tr));
    if (!v) return NULL;
    v->size = size;
    v->capacity = size > 0 ? size : 4;
    v->ptr = malloc(sizeof(transaction) * v->capacity);
    if (!v->ptr) {
        free(v);
        return NULL;
    }
    return v;
}

// Очищаем память вектора
int vec_tr_destroy(vec_tr *vec) {
    if (!vec) return -1;
    free(vec->ptr);
    free(vec);
    return 0;
}

// Меняем размер, при нехватке памяти увеличиваем емкость в 2 раза
int vec_tr_set_size(vec_tr *vec, int size) {
    if (!vec || size < 0) return -1;
    if (size > vec->capacity) {
        int new_cap = size * 2;
        transaction *p = realloc(vec->ptr, sizeof(transaction) * new_cap);
        if (!p) return -1;
        vec->ptr = p;
        vec->capacity = new_cap;
    }
    vec->size = size;
    return 0;
}

// Возвращаем указатель на размер
int *vec_tr_get_size(vec_tr *vec) {
    return vec ? &vec->size : NULL;
}

// Запись транзакции по индексу
int vec_tr_set(vec_tr *vec, int index, transaction value) {
    if (!vec || !vec->ptr || index < 0 || index >= vec->size) return -1;
    vec->ptr[index] = value;
    return 0;
}

// Получение транзакции по индексу
transaction *vec_tr_get(vec_tr *vec, int index) {
    if (!vec || !vec->ptr || index < 0 || index >= vec->size) return NULL;
    return &vec->ptr[index];
}

// Добавление новой транзакции в конец
int vec_tr_push(vec_tr *vec, transaction value) {
    if (!vec) return -1;
    int cur = vec->size;
    if (vec_tr_set_size(vec, cur + 1) != 0) return -1;
    vec->ptr[cur] = value;
    return 0;
}
