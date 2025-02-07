
#include "array.h"

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

