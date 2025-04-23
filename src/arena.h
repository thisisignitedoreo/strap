
// NOTE: this was reimplemented as a linked list (at 2024-02-28).
// IF you see any errors referring to `arena_new(size_t)`
// it means you should update your sources accordingly

// NOTE: I did not use "linked_list.h" here because it
// depends on "arena.h" which will be a cyclic inclusion.

#ifndef ARENA_H_
#define ARENA_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define ARENA_DEF_SIZE (1024*8)

typedef struct Region {
    char *memory;
    size_t cursor;
    size_t size;
    struct Region *next;
} Region;

typedef struct {
    Region *begin, *end;
} Arena;

void* arena_malloc(Arena* arena, size_t size);
void arena_free(Arena* arena);

char* arena_printf(Arena* arena, char* fmt, ...);
#define arena_sprintf(a, ...) sv(arena_printf(a, __VA_ARGS__))

#endif // ARENA_H_
