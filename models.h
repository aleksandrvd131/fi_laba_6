#pragma once

// Дата (день, месяц, год)
typedef struct {
    int day;
    int month;
    int year;
} date;

// Время
typedef struct {
    int hour;
    int minute;
    int second;
} time;

// Транзакция: дата, время, тип (1 - приход, 0 - расход), сумма в рублях
typedef struct {
    date dt;
    time t;
    int type;
    double rub;
} transaction;
