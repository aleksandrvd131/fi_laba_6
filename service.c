#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "service.h"

// Поиск клиента в векторе по его ID
client *get_by_id(vec_cl *clients, int id) {
    if (!clients) return NULL;
    for (int i = 0; i < *vec_cl_get_size(clients); i++) {
        client *c = vec_cl_get(clients, i);
        if (c && c->id == id) return c;
    }
    return NULL;
}

// Компаратор для сортировки по количеству транзакций
int cmp(client *x, client *y) {
    if (!x || !y) return 0;
    return *vec_tr_get_size(x->transactions) > *vec_tr_get_size(y->transactions);
}

// Пузырьковая сортировка клиентов
void sort_clients(vec_cl *clients, int (*cmp_func)(client *x, client *y)) {
    if (!clients || !cmp_func) return;
    int n = *vec_cl_get_size(clients);
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            client *a = vec_cl_get(clients, j);
            client *b = vec_cl_get(clients, j + 1);
            if (cmp_func(a, b)) {
                client tmp = *a;
                *a = *b;
                *b = tmp;
            }
        }
    }
}

// Условие для бинарного поиска
int bs_cmp(client *x, int val) {
    return x ? (*vec_tr_get_size(x->transactions) < val) : 0;
}

// Бинарный поиск границы в отсортированном векторе клиентов
cl_bs_res *cl_binary_search(vec_cl *clients, int l, int r, int val, int (*bs_cmp_func)(client *x, int val)) {
    if (!clients || !bs_cmp_func) return NULL;
    while (r - l > 1) {
        int m = l + (r - l) / 2;
        if (bs_cmp_func(vec_cl_get(clients, m), val)) l = m;
        else r = m;
    }
    cl_bs_res *res = malloc(sizeof(cl_bs_res));
    if (!res) return NULL;
    res->l = vec_cl_get(clients, l);
    res->r = vec_cl_get(clients, r >= *vec_cl_get_size(clients) ? r - 1 : r);
    return res;
}

// Разбор даты из строки ДД.ММ.ГГ или ДД.ММ.ГГГГ
int date_parse(const char *str, date *out) {
    if (!str || !out) return 0;
    if (sscanf(str, "%d.%d.%d", &out->day, &out->month, &out->year) != 3) return 0;
    if (out->year < 100) out->year += (out->year < 70 ? 2000 : 1900);
    return 1;
}

// Сравнение дат: возвращает <0 если a < b, 0 если равны, >0 если a > b
int date_cmp(date a, date b) {
    if (a.year != b.year) return a.year - b.year;
    if (a.month != b.month) return a.month - b.month;
    return a.day - b.day;
}

// Загрузка клиентов и их операций из файла
int load_clients_from_file(const char *filename, vec_cl **out_clients) {
    FILE *f = fopen(filename, "r");
    if (!f) return -1;

    vec_cl *clients = vec_cl_init(0);
    client cl;
    char s_date[32], s_time[32], s_type[32], s_rub[32];

    while (fscanf(f, "%d", &cl.id) == 1) {
        if (fscanf(f, "%63s %63s %63s", cl.surname, cl.name, cl.patronym) != 3) break;
        int tx_count = 0;
        if (fscanf(f, "%d", &tx_count) != 1) break;

        cl.transactions = vec_tr_init(0);
        for (int i = 0; i < tx_count; i++) {
            transaction tr;
            if (fscanf(f, "%31s %31s %31s %31s", s_date, s_time, s_type, s_rub) != 4) break;
            date_parse(s_date, &tr.dt);
            sscanf(s_time, "%d:%d:%d", &tr.t.hour, &tr.t.minute, &tr.t.second);

            // 1 - приход, 0 - расход
            tr.type = (strstr(s_type, "+") || strstr(s_type, "рих") || strstr(s_type, "РИХ") || strstr(s_type, "inc")) ? 1 : 0;

            // заменяем запятую на точку, если попалась в сумме
            char *comma = strchr(s_rub, ',');
            if (comma) *comma = '.';
            tr.rub = atof(s_rub);

            vec_tr_push(cl.transactions, tr);
        }
        vec_cl_push(clients, cl);
    }

    fclose(f);
    *out_clients = clients;
    return 0;
}

// Подсчет доходов, расходов и баланса за выбранный период
client_stat *calculate_period_stats(vec_cl *clients, date from_date, date to_date) {
    if (!clients) return NULL;
    int n = *vec_cl_get_size(clients);
    client_stat *stats = malloc(sizeof(client_stat) * (n > 0 ? n : 1));
    if (!stats) return NULL;

    for (int i = 0; i < n; i++) {
        client *c = vec_cl_get(clients, i);
        stats[i].cl = c;
        stats[i].income = 0;
        stats[i].expense = 0;
        stats[i].total_tx_count = c->transactions ? *vec_tr_get_size(c->transactions) : 0;
        stats[i].period_tx_count = 0;

        for (int j = 0; j < stats[i].total_tx_count; j++) {
            transaction *tr = vec_tr_get(c->transactions, j);
            if (date_cmp(tr->dt, from_date) >= 0 && date_cmp(tr->dt, to_date) <= 0) {
                stats[i].period_tx_count++;
                if (tr->type == 1) stats[i].income += tr->rub;
                else stats[i].expense += tr->rub;
            }
        }
        stats[i].balance = stats[i].income - stats[i].expense;
    }
    return stats;
}
