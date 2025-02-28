
#include "array.h"

#include <stdio.h>
#include <stdarg.h>

array_implement(I32Array, int32_t)
array_implement(U32Array, uint32_t)
array_implement(FloatArray, float)
array_implement(DoubleArray, double)
array_implement(StringArray, String)
array_implement(CStrArray, char*)
array_implement(StringBuilder, char)
array_implement(SBArray, StringBuilder*)

void StringBuilder_push_string(StringBuilder* array, String string) {
    StringBuilder_push_many(array, string.bytes, string.size);
}

void StringBuilder_push_cstring(StringBuilder* array, char* string) {
    while (*string) StringBuilder_push(array, *(string++));
}

void StringBuilder_printf(StringBuilder* array, const char* fmt, ...) {
    va_list args;

    va_start(args, fmt);
    int n = vsnprintf(NULL, 0, fmt, args);
    va_end(args);
    
    if (n < 0) return;
    
    while (n >= (int) (array->capacity - array->size)) {
        char* new_items = arena_malloc(array->allocator, array->capacity * 2);
        memcpy(new_items, array->items, array->capacity);
        array->items = new_items;
        array->capacity *= 2;
    }
    
    va_start(args, fmt);
    n = vsnprintf(array->items + array->size, array->capacity - array->size, fmt, args);
    va_end(args);
    
    array->size += n;
}

