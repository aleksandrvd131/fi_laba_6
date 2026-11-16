#pragma once
#include "vec_tr.h"

// Структура данных клиента
typedef struct {
    int id;
    char surname[64];
    char name[64];
    char patronym[64];
    vec_tr *transactions; // динамический массив транзакций
} client;
