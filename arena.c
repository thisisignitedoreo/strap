
#include "arena.h"

#include <stdlib.h>

Arena* arena_new(size_t size) {
    Arena* arena = malloc(sizeof(Arena));
    arena->memory = malloc(size);
    arena->cursor = 0;
    arena->size = size;
    return arena;
}

void* arena_allocate(Arena* arena, size_t size) {
    if (arena->cursor >= arena->size) return NULL;
    void* ptr = arena->memory + arena->cursor;
    arena->cursor += size;
    return ptr;
}

void arena_resize(Arena* arena, size_t new_size) {
    arena->size = new_size;
    arena->memory = realloc(arena->memory, new_size);
}

bool arena_done(Arena* arena) {
    return arena->cursor >= arena->size;
}

void arena_free(Arena* arena) {
    free(arena->memory);
    free(arena);
}
