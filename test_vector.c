#include <stdio.h>
#include <assert.h>
#include "vec_tr.h"
#include "vec_cl.h"

int main(void) {
    printf("Тестирование векторов...\n");

    // 1. Проверяем вектор транзакций
    vec_tr *vt = vec_tr_init(0);
    assert(vt != NULL);

    transaction t1 = {{1, 5, 2024}, {10, 0, 0}, 1, 1500.0};
    transaction t2 = {{2, 5, 2024}, {12, 0, 0}, 0, 500.0};
    vec_tr_push(vt, t1);
    vec_tr_push(vt, t2);

    assert(*vec_tr_get_size(vt) == 2);
    assert(vec_tr_get(vt, 0)->rub == 1500.0);
    assert(vec_tr_get(vt, 1)->type == 0);
    vec_tr_destroy(vt);

    // 2. Проверяем вектор клиентов и вложенные векторы транзакций
    vec_cl *vc = vec_cl_init(0);
    client c1 = {101, "Иванов", "Иван", "Иванович", vec_tr_init(0)};
    vec_tr_push(c1.transactions, t1);
    vec_cl_push(vc, c1);

    assert(*vec_cl_get_size(vc) == 1);
    assert(vec_cl_get(vc, 0)->id == 101);
    assert(*vec_tr_get_size(vec_cl_get(vc, 0)->transactions) == 1);

    // Освобождаем память (включая транзакции клиентов)
    vec_cl_destroy(vc);

    printf("Все тесты вектора успешно пройдены!\n");
    return 0;
}
