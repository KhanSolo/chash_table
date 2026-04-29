#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "hash_table.h"

//#include "dbg_alloc.h"

/*
void find_collision() {
    const char* base = "key";
    int target = ht_bucket(base);

    char buf[32];

    for (int i = 0; i < 100000; i++) {
        sprintf(buf, "key%d", i);
        if (strcmp(buf, base) != 0 && ht_bucket(buf) == target) {
            printf("Collision found!\n");
            printf("%s and %s -> bucket %d\n", base, buf, target);
            return;
        }
    }
}
*/

int main(void)
{
    hash_table_t ht;
    ht_init(&ht);
    
    //find_collision();

    // базовые вставки
    ht_add(&ht, "num1", "Hello!");
    ht_add(&ht, "num2", "World!");
    ht_add(&ht, "num3", "HashTable");

    // проверка получения
    ht_print_value(&ht, "num1");
    ht_print_value(&ht, "num2");
    ht_print_value(&ht, "num3");
    ht_print_value(&ht, "num4"); // не существует

    // замена значения
    ht_add(&ht, "num2", "Updated!");
    ht_print_value(&ht, "num2");

    // добавим больше данных
    ht_add(&ht, "apple", "fruit");
    ht_add(&ht, "car", "vehicle");
    ht_add(&ht, "dog", "animal");

    ht_print_value(&ht, "apple");
    ht_print_value(&ht, "car");
    ht_print_value(&ht, "dog");

    // проверка коллизий (искусственно)
    ht_add(&ht, "key", "first");
    ht_add(&ht, "key305", "second"); // в тот же бакет при HASH_LEN 997

    ht_print_value(&ht, "key");
    ht_print_value(&ht, "key305");

    ht_debug_print(&ht);

    ht_free(&ht);

    //dbg_report_leaks();

    return 0;
}