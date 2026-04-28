#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//#include "dbg_alloc.h"

#define HASH_LEN 997

typedef struct ht_entry {
    char *key;
    char *value;
    unsigned long hash;
    struct ht_entry *next;
} ht_entry;

typedef struct hash_table {
    ht_entry *buckets[HASH_LEN];
} hash_table_t;

unsigned long djb2(const char * str) {
    unsigned long hash = 5381;
    int c;
    while((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

ht_entry* ht_entry_new(const char* key, const char* value, unsigned long hash) {
    ht_entry* entry = malloc(sizeof(ht_entry));
    if (!entry) return NULL;

    char* dup_key = strdup(key);
    if (!dup_key) {
        free(entry);
        return NULL;
    }

    char* dup_val = strdup(value);
    if (!dup_val) {
        free(dup_key);
        free(entry);
        return NULL;
    }

    entry->key = dup_key;
    entry->value = dup_val;
    entry->hash = hash;
    entry->next = NULL;
    return entry;
}

void ht_add(hash_table_t *ht, const char* key, const char* value) {
    unsigned long hash = djb2(key);
    int bucket = hash % HASH_LEN;
    ht_entry* existing = ht->buckets[bucket];
    if (existing) {
        while (existing) {
            if (existing->hash == hash && strcmp(existing->key, key) == 0) {
                char* dup_val = strdup(value);
                if (!dup_val) {
                    return;
                }
                free(existing->value);
                existing->value = dup_val;
                return;
            }
            if (!existing->next) {
                ht_entry* new_entry = ht_entry_new(key, value, hash);
                if (!new_entry) {
                    return;
                }
                existing->next = new_entry;
                return;
            }
            existing = existing->next;
        }
    } else {
        ht_entry* new_entry = ht_entry_new(key, value, hash);
        if (!new_entry) {
            return;
        }
        ht->buckets[bucket] = new_entry;
    }
}

char* ht_get(hash_table_t *ht, const char* key) {
    unsigned long hash = djb2(key);
    int bucket = hash % HASH_LEN;
    ht_entry* existing = ht->buckets[bucket];
    while (existing) {
        if (existing->hash == hash && strcmp(existing->key, key) == 0) {
            return existing->value;
        }
        existing = existing->next;
    }
    return NULL;
}

void ht_free(hash_table_t *ht) {
    for (int i = 0; i < HASH_LEN; ++i) {
        ht_entry* cur = ht->buckets[i];
        while (cur) {
            ht_entry* tmp = cur;
            cur = cur->next;
            free(tmp->key);
            free(tmp->value);
            free(tmp);
        }
        ht->buckets[i] = NULL;
    }
}

void ht_print_value(hash_table_t *ht, const char* key) {
    char* val = ht_get(ht, key);
    printf("for %s: [%s]\n", key, val ? val : "NULL");
}

void ht_debug_print(hash_table_t *ht) {
    for (int i = 0; i < HASH_LEN; i++) {
        ht_entry* cur = ht->buckets[i];
        if (cur) {
            printf("[%d]: ", i);
            while (cur) {
                printf("(%s -> %s) -> ", cur->key, cur->value);
                cur = cur->next;
            }
            printf("NULL\n");
        }
    }
}

void ht_init(hash_table_t *ht) {
    memset(ht->buckets, 0, sizeof(ht->buckets));
}

int ht_bucket(const char* key) {
    return djb2(key) % HASH_LEN;
}

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