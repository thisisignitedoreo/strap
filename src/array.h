
#ifndef ARRAY_H_
#define ARRAY_H_

#include <stdint.h>
#include <string.h>

#include "arena.h"
#include "stringview.h"

#define ARRAY_CAPACITY 1

#define array_define(array_name, array_type) \
    typedef struct { \
        array_type* items; \
        size_t size; \
        size_t capacity; \
        Arena* allocator; \
    } array_name; \
    \
    array_name* array_name##_new(Arena* arena); \
    array_type array_name##_get(array_name* array, size_t index); \
    void array_name##_set(array_name* array, size_t index, array_type item); \
    void array_name##_push(array_name* array, array_type object); \
    void array_name##_push_many(array_name* array, array_type* objects, size_t size); \
    void array_name##_pop(array_name* array); \
    void array_name##_remove_at(array_name* array, size_t index); \
    void array_name##_insert_at(array_name* array, array_type item, size_t index);

#define array_implement(array_name, array_type) \
    array_name* array_name##_new(Arena* arena) { \
        array_name* array = arena_allocate(arena, sizeof(array_name)); \
        if (array == NULL) return NULL; \
        array->items = arena_allocate(arena, ARRAY_CAPACITY * sizeof(array_type)); \
        if (array->items == NULL) return NULL; \
        array->size = 0; array->capacity = ARRAY_CAPACITY; \
        array->allocator = arena; \
        return array; \
    } \
    array_type array_name##_get(array_name* array, size_t index) { \
        if (index >= array->size) return (array_type) {0}; \
        return array->items[index]; \
    } \
    void array_name##_set(array_name* array, size_t index, array_type item) { \
        if (index >= array->size) return; \
        array->items[index] = item; \
    } \
    void array_name##_push(array_name* array, array_type object) { \
        if (array->size == array->capacity) { \
            array_type* new_items = arena_allocate(array->allocator, array->capacity * 2 * sizeof(array_type)); \
            memcpy(new_items, array->items, array->capacity * sizeof(array_type)); \
            array->items = new_items; \
            array->capacity *= 2; \
        } \
        array->items[array->size++] = object; \
    } \
    void array_name##_push_many(array_name* array, array_type* objects, size_t size) { \
        for (size_t i = 0; i < size; i++) array_name##_push(array, objects[i]); \
    } \
    void array_name##_pop(array_name* array) { array->size--; } \
    void array_name##_remove_at(array_name* array, size_t index) { \
        for (size_t i = index + 1; i < array->size; i++) { \
            array->items[index - 1] = array->items[index]; \
        } \
        array_name##_pop(array); \
    } \
    void array_name##_insert_at(array_name* array, array_type item, size_t index) { \
        array_name##_push(array, item); \
        for (size_t i = index; i < array->size - 1; i++) { \
            array->items[index + 1] = array->items[index]; \
        } \
        array->items[index] = item; \
    }

array_define(I32Array, int32_t)
array_define(U32Array, uint32_t)
array_define(FloatArray, float)
array_define(DoubleArray, double)
array_define(StringArray, String)
array_define(StringBuilder, char)
array_define(SBArray, StringBuilder*)

void StringBuilder_push_string(StringBuilder* array, String string);
void StringBuilder_push_cstring(StringBuilder* array, char* string);

#define array_foreach(array, i) for (size_t i = 0; i < array->size; i++)

#endif // ARRAY_H_
