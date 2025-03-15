
// NOTE: this was reimplemented as a system of macros (on 2025-03-01)
// IF you see any errors referring to `I32Array_new(Arena*)` or similar
// it means you should update your sources accordingly

#ifndef ARRAY_H_
#define ARRAY_H_

#include <stdint.h>
#include <string.h>

#include "arena.h"
#include "stringview.h"

#define ARRAY_CAPACITY 1

#define define_array_struct(array_name, array_type) \
    typedef struct { \
        size_t size, capacity; \
        array_type* data; \
        Arena* allocator; \
    } array_name;

define_array_struct(I32Array, int32_t)
define_array_struct(U32Array, uint32_t)
define_array_struct(FloatArray, float)
define_array_struct(DoubleArray, double)
define_array_struct(StringArray, String)
define_array_struct(CStrArray, char*)
define_array_struct(StringBuilder, char)
define_array_struct(SBArray, StringBuilder*)

void* array_resize(Arena* arena, void* data, size_t* capacity, size_t stride);
#define array_push(array) \
    ((array)->size == (array)->capacity ? \
        (array)->data = array_resize((array)->allocator, (array)->data, &(array)->capacity, sizeof(*(array)->data)), \
        (array)->data + (array)->size++ : \
        (array)->data + (array)->size++ \
    )
#define array_pop(array) *((array)->data + (array)->size--)

#define array_set(array, element) ((array)->data + element)
#define array_get(array, element) (*((array)->data + element))

void array_remove_(char* data, size_t element, size_t* size, size_t stride);
#define array_remove(array, element) (array_remove_((char*) (array)->data, (element), &(array)->size, sizeof(*(array)->data)))

void* array_add_(Arena* arena, char* data, size_t element, size_t* size, size_t stride);
#define array_add(array, element) (array_push(array), array_add_((array)->allocator, (array)->data, (element), &(array)->size, sizeof(*(array)->data)))

void* array_new_(size_t struct_size, Arena* arena);
#define array_new(array_type, arena) array_new_(sizeof(array_type), arena)

#define array_foreach(array, i) for (size_t i = 0; i < array->size; i++)

void array_sb_push_cstring(StringBuilder* sb, char* str);
void array_sb_push_string(StringBuilder* sb, String str);
void array_sb_printf(StringBuilder* sb, char* fmt, ...);

#endif // ARRAY_H_
