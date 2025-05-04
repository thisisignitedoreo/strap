
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

void array_remove_(char* data, size_t element, size_t* size, size_t stride) {
    memmove(data + element*stride, data + (element+1)*stride, stride*(*size - element - 1));
    *size -= 1;
}

void* array_add_(Arena* arena, char* data, size_t element, size_t* size, size_t stride) {
    *size -= 1;
    if (*size != element) {
        void* temp = arena_malloc(arena, stride*(*size - element));
        memcpy(temp, data + element*stride, stride*(*size - element));
        memcpy(data + (element+1)*stride, temp, stride*(*size - element));
    }
    *size += 1;
    return data + element*stride;
}

void* array_new(Arena* arena) {
    // Who cares what type checker thinks? A pointer is a pointer (and is 8 bytes (usually))
    I32Array* s = arena_malloc(arena, sizeof(I32Array));
    memset(s, 0, sizeof(I32Array));
    s->allocator = arena;
    return s;
}

void array_sb_push_cstring(StringBuilder* sb, char* str) {
    if (!sb->data) sb->data = array_resize(sb->allocator, sb->data, &sb->capacity, sizeof(*sb->data));
    for (size_t i = 0; i < strlen(str); i++) *array_push(sb) = str[i];
}

void array_sb_push_string(StringBuilder* sb, String str) {
    if (!sb->data) sb->data = array_resize(sb->allocator, sb->data, &sb->capacity, sizeof(*sb->data));
    for (size_t i = 0; i < str.size; i++) *array_push(sb) = str.bytes[i];
}

void array_sb_printf(StringBuilder* sb, char* fmt, ...) {
    if (!sb->data) sb->data = array_resize(sb->allocator, sb->data, &sb->capacity, sizeof(*sb->data));
    
    va_list args;

    va_start(args, fmt);
    int n = vsnprintf(NULL, 0, fmt, args);
    va_end(args);

    if (n < 0) return;

    while ((size_t) n >= sb->capacity - sb->size) {
        size_t new_cap = sb->capacity ? sb->capacity * 2 : ARRAY_CAPACITY;
        char* new_items = arena_malloc(sb->allocator, new_cap*sizeof(*sb->data));
        if (sb->data) memcpy(new_items, sb->data, sb->capacity*sizeof(*sb->data));
        sb->data = new_items;
        sb->capacity = new_cap;
    }
    
    va_start(args, fmt);
    vsnprintf(sb->data + sb->size, sb->capacity - sb->size, fmt, args);
    va_end(args);

    sb->size += n;
}

