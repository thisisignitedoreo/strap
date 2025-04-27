
#include "arena.h"

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

void* arena_malloc(Arena* arena, size_t size) {
    size += -size & 7;  // FIXME: a dirty fix for structure alignment
    if (arena->begin == NULL && arena->end == NULL) {
        Region* new_region = malloc(sizeof(Region));

        new_region->cursor = 0;
        if (size > ARENA_DEF_SIZE) new_region->size = size;
        else new_region->size = ARENA_DEF_SIZE;
        new_region->memory = malloc(new_region->size);
        new_region->next = NULL;
        
        arena->begin = new_region;
        arena->end = new_region;
    }
    if (arena->end->next == NULL && arena->end->size - arena->end->cursor < size) {
        Region* new_region = malloc(sizeof(Region));

        new_region->cursor = 0;
        if (size > ARENA_DEF_SIZE) new_region->size = size;
        else new_region->size = ARENA_DEF_SIZE;
        new_region->memory = malloc(new_region->size);
        new_region->next = NULL;

        arena->end->next = new_region;
        arena->end = new_region;
    }
    void* ptr = arena->end->memory + arena->end->cursor;
    arena->end->cursor += size;
    return ptr;
}

void arena_free(Arena* arena) {
    if (arena->begin == NULL && arena->end == NULL) return;
    for (Region* r = arena->begin; r;) {
        Region* r_next = r->next;
        free(r->memory);
        free(r);
        r = r_next;
    }
}

char* arena_printf(Arena* arena, char* fmt, ...) {
    va_list args;

    va_start(args, fmt);
    int n = vsnprintf(NULL, 0, fmt, args);
    va_end(args);

    char* mem = arena_malloc(arena, n+1);

    va_start(args, fmt);
    vsnprintf(mem, n, fmt, args);
    va_end(args);

    return mem;
}
