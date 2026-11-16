#pragma once
#include "client.h"
#include "models.h"
#include "vec_cl.h"
#include "vec_tr.h"

// Результат бинарного поиска
typedef struct {
    client *l;
    client *r;
} cl_bs_res;

// Статистика клиента за выбранный период
typedef struct {
    client *cl;
    double income;
    double expense;
    double balance;
    int period_tx_count;
    int total_tx_count;
} client_stat;

// Поиск и сортировка
client *get_by_id(vec_cl *clients, int id);
int cmp(client *x, client *y);
void sort_clients(vec_cl *clients, int (*cmp)(client *x, client *y));
int bs_cmp(client *x, int val);
cl_bs_res *cl_binary_search(vec_cl *clients, int l, int r, int val, int (*bs_cmp)(client *x, int val));

// Работа с датами
int date_parse(const char *str, date *out_date);
int date_cmp(date a, date b);

// Загрузка данных и расчет балансов
int load_clients_from_file(const char *filename, vec_cl **out_clients);
client_stat *calculate_period_stats(vec_cl *clients, date from_date, date to_date);
