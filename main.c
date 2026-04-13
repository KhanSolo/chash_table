#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define HASH_LEN 997

typedef struct ht {
    const char * key;    
    char * value;
    unsigned long hash;
    struct ht * next;
} ht;

ht * hash_table[HASH_LEN];

unsigned long djb2(const char * str) {
    unsigned long hash = 5381;
    char c;
    while((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

ht* ht_new_entry(const char* key, char* value, unsigned long hash){
    ht* current = (ht*)malloc(sizeof(ht));
    current->key = key;
    current->value = value;
    current->hash = hash;
    current->next = NULL;
    return current;
}

void ht_add(const char* key, char* value){
    unsigned long hash = djb2(key);
    int bucket = hash % HASH_LEN;    
    ht* existing = hash_table[bucket];
    if (existing) {
        while (existing) {
            if (existing->hash == hash &&
                strcmp(existing->key, key) == 0) { // ключи равны 
                existing->value = value; // заменяем
                return;
            }
            if (!existing->next) {
                ht* current = ht_new_entry(key, value, hash);
                existing->next = current;
                return;
            }
            existing = existing->next;
        }
    } else {
        ht* current = ht_new_entry(key, value, hash);
        hash_table[bucket] = current;
    }
}

char* ht_get(const char* key){
    unsigned long hash = djb2(key);
    int bucket = hash % HASH_LEN;

    ht* existing = hash_table[bucket];
    if (existing) {
        while (existing) {
            if(0 == strcmp(existing->key, key)){ // ключи равны            
                return existing->value; 
            }
            existing = existing->next;
        }
    }
    return NULL;
}

void ht_print_value(const char* key) {
    char* val = ht_get(key);
    printf("for %s: [%s]\n", key, val ? val : "NULL");
}

void ht_init() {
    // глобальная ht будет заполнена нулями
    // если перенести в локальные переменные, то нужно заполнять
    memset(hash_table, 0, sizeof(hash_table));
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
    ht_init();
    
    //find_collision(); 

    // базовые вставки
    ht_add("num1", "Hello!");
    ht_add("num2", "World!");
    ht_add("num3", "HashTable");

    // проверка получения
    ht_print_value("num1");
    ht_print_value("num2");
    ht_print_value("num3");
    ht_print_value("num4"); // не существует

    // замена значения
    ht_add("num2", "Updated!");
    ht_print_value("num2");

    // добавим больше данных
    ht_add("apple", "fruit");
    ht_add("car", "vehicle");
    ht_add("dog", "animal");

    ht_print_value("apple");
    ht_print_value("car");
    ht_print_value("dog");    

    // проверка коллизий (искусственно)
    ht_add("key", "first");
    ht_add("key305", "second"); // в тот же бакет при HASH_LEN 997

    ht_print_value("key");
    ht_print_value("key305");

    return 0;
}