#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "service.h"

int main(int argc, char *argv[]) {
    char file[256], s_from[32], s_to[32];
    date d_from, d_to;

    // Считываем аргументы командной строки или просим ввести вручную
    if (argc >= 4) {
        strncpy(file, argv[1], sizeof(file) - 1);
        strncpy(s_from, argv[2], sizeof(s_from) - 1);
        strncpy(s_to, argv[3], sizeof(s_to) - 1);
    } else {
        printf("Имя файла: ");
        if (scanf("%255s", file) != 1) return 1;
        printf("Дата начала (ДД.ММ.ГГ): ");
        if (scanf("%31s", s_from) != 1) return 1;
        printf("Дата конца (ДД.ММ.ГГ): ");
        if (scanf("%31s", s_to) != 1) return 1;
    }

    if (!date_parse(s_from, &d_from) || !date_parse(s_to, &d_to)) {
        printf("Ошибка в формате даты.\n");
        return 1;
    }

    vec_cl *clients = NULL;
    if (load_clients_from_file(file, &clients) != 0 || !clients) {
        printf("Не удалось открыть файл %s\n", file);
        return 1;
    }

    int n = *vec_cl_get_size(clients);
    printf("Загружено клиентов: %d\n", n);

    // Считаем доходы и расходы за период
    client_stat *stats = calculate_period_stats(clients, d_from, d_to);
    if (!stats || n == 0) {
        vec_cl_destroy(clients);
        return 0;
    }

    // Выводим общую сводку
    printf("\nID       ФИО                              Доход        Расход       Баланс\n");
    printf("--------------------------------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        char fio[256];
        snprintf(fio, sizeof(fio), "%s %s %s", stats[i].cl->surname, stats[i].cl->name, stats[i].cl->patronym);
        printf("%-8d %-32s %12.2f %12.2f %12.2f\n",
               stats[i].cl->id, fio, stats[i].income, stats[i].expense, stats[i].balance);
    }

    // Ищем минимальный баланс
    double min_bal = stats[0].balance;
    for (int i = 1; i < n; i++) {
        if (stats[i].balance < min_bal) min_bal = stats[i].balance;
    }

    // Выводим всех клиентов с минимальным балансом
    printf("\n--- Клиенты с минимальным балансом (%.2f руб.) ---\n", min_bal);
    for (int i = 0; i < n; i++) {
        if (fabs(stats[i].balance - min_bal) < 1e-4) {
            printf("ID: %d | %s %s %s | Баланс: %.2f руб.\n",
                   stats[i].cl->id, stats[i].cl->surname, stats[i].cl->name, stats[i].cl->patronym, stats[i].balance);
        }
    }

    // Освобождаем память
    free(stats);
    vec_cl_destroy(clients);
    return 0;
}
