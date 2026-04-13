#ifndef DBG_ALLOC_H
#define DBG_ALLOC_H

#include <stdio.h>
#include <stdlib.h>

typedef struct MemBlock {
    void* ptr;
    size_t size;
    const char* file;
    int line;
    int freed;
    struct MemBlock* next;
} MemBlock;

static MemBlock* head = NULL;

void* dbg_malloc(size_t size, const char* file, int line) {
    void* ptr = malloc(size);

    MemBlock* b = (MemBlock*)malloc(sizeof(MemBlock));
    b->ptr = ptr;
    b->size = size;
    b->file = file;
    b->line = line;
    b->freed = 0;
    b->next = head;
    head = b;

    printf("[ALLOC] %p (%zu bytes) at %s:%d\n", ptr, size, file, line);

    return ptr;
}

void dbg_free(void* ptr, const char* file, int line) {
    MemBlock* cur = head;

    while (cur) {
        if (cur->ptr == ptr) {
            if (cur->freed) {
                printf("[DOUBLE FREE] %p at %s:%d (original %s:%d)\n",
                       ptr, file, line, cur->file, cur->line);
                return;
            }

            cur->freed = 1;
            free(ptr);

            printf("[FREE] %p at %s:%d\n", ptr, file, line);
            return;
        }
        cur = cur->next;
    }

    printf("[INVALID FREE] %p at %s:%d\n", ptr, file, line);
}

#define malloc(x) dbg_malloc(x, __FILE__, __LINE__)
#define free(x)   dbg_free(x, __FILE__, __LINE__)

#endif