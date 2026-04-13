// dbg_alloc.h

#ifndef DBG_ALLOC_H
#define DBG_ALLOC_H

#include <stdio.h>
#include <stdlib.h>

typedef struct MemBlock {
    void* ptr;
    size_t size;
    const char* file;
    int line;
    struct MemBlock* next;
} MemBlock;

static MemBlock* head = NULL;

// Временно отключаем макросы, чтобы внутри функций вызывать настоящие malloc/free
#ifdef malloc
#undef malloc
#endif
#ifdef free
#undef free
#endif

void* dbg_malloc(size_t size, const char* file, int line) {
    // Используем настоящий malloc через (malloc) для подавления макроса
    void* ptr = (malloc)(size);
    if (!ptr) return NULL;

    MemBlock* b = (MemBlock*)(malloc)(sizeof(MemBlock));
    if (!b) {
        (free)(ptr);
        return NULL;
    }

    b->ptr = ptr;
    b->size = size;
    b->file = file;
    b->line = line;
    b->next = head;
    head = b;

    printf("[ALLOC] %p (%zu bytes) at %s:%d\n", ptr, size, file, line);
    return ptr;
}

void dbg_free(void* ptr, const char* file, int line) {
    if (!ptr) return;

    MemBlock* cur = head;
    MemBlock* prev = NULL;

    while (cur) {
        if (cur->ptr == ptr) {
            // Нашли блок — удаляем его из списка
            if (prev)
                prev->next = cur->next;
            else
                head = cur->next;

            printf("[FREE] %p at %s:%d (allocated at %s:%d)\n",
                   ptr, file, line, cur->file, cur->line);

            (free)(ptr);          // освобождаем пользовательскую память
            (free)(cur);          // освобождаем учётный блок
            return;
        }
        prev = cur;
        cur = cur->next;
    }

    // Указатель не найден в списке — возможно, двойное освобождение или чужой указатель
    printf("[INVALID FREE] %p at %s:%d\n", ptr, file, line);
    // В отладочной версии лучше не освобождать, чтобы не маскировать ошибки
}

void dbg_report_leaks(void) {
    MemBlock* cur = head;
    int leak_count = 0;
    while (cur) {
        printf("[LEAK] %p (%zu bytes) allocated at %s:%d\n",
               cur->ptr, cur->size, cur->file, cur->line);
        leak_count++;
        cur = cur->next;
    }
    if (leak_count == 0)
        printf("[OK] No memory leaks detected.\n");
    else
        printf("[LEAKS] Total %d unreleased block(s).\n", leak_count);
}

// Возвращаем макросы для перехвата malloc/free в пользовательском коде
#define malloc(x) dbg_malloc(x, __FILE__, __LINE__)
#define free(x)   dbg_free(x, __FILE__, __LINE__)

#endif // DBG_ALLOC_H