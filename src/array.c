
#include "array.h"

#include <stdio.h>
#include <stdarg.h>

void* array_resize(Arena* arena, void* data, size_t* capacity, size_t stride) {
    size_t new_capacity = *capacity ? (*capacity)*2 : ARRAY_CAPACITY;
    void* new_data = arena_malloc(arena, stride * new_capacity);
    if (data) memcpy(new_data, data, stride * (*capacity));
    *capacity = new_capacity;
    return new_data;
}

void* array_new_(size_t struct_size, Arena* arena) {
    // Look. There is no other way. We anyway do not care.
    I32Array* s = arena_malloc(arena, struct_size);
    memset(s, 0, struct_size);
    s->allocator = arena;
    return s;
}

void array_sb_push_cstring(StringBuilder* sb, char* str) {
    if (!sb->data) array_resize(sb->allocator, sb->data, &sb->capacity, sizeof(*sb->data));
    for (size_t i = 0; i < strlen(str); i++) *array_push(sb) = str[i];
}

void array_sb_push_string(StringBuilder* sb, String str) {
    if (!sb->data) array_resize(sb->allocator, sb->data, &sb->capacity, sizeof(*sb->data));
    for (size_t i = 0; i < str.size; i++) *array_push(sb) = str.bytes[i];
}

void array_sb_printf(StringBuilder* sb, char* fmt, ...) {
    if (!sb->data) array_resize(sb->allocator, sb->data, &sb->capacity, sizeof(*sb->data));
    
    va_list args;

    va_start(args, fmt);
    int n = vsnprintf(NULL, 0, fmt, args);
    va_end(args);

    if (n < 0) return;

    while ((size_t) n >= sb->capacity - sb->size) {
        char* new_items = arena_malloc(sb->allocator, sb->capacity * 2);
        memcpy(new_items, sb->data, sb->capacity);
        sb->data = new_items;
        sb->capacity *= 2;
    }
    
    va_start(args, fmt);
    vsnprintf(sb->data + sb->size, sb->capacity - sb->size, fmt, args);
    va_end(args);

    sb->size += n;
}

