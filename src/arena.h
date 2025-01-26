
#ifndef ARENA_H_
#define ARENA_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    void* memory;
    size_t cursor;
    size_t size;
} Arena;

Arena* arena_new(size_t size);
void* arena_allocate(Arena* arena, size_t size);
void arena_resize(Arena* arena, size_t new_size);
bool arena_done(Arena* arena);
void arena_free(Arena* arena);

#endif // ARENA_H_
