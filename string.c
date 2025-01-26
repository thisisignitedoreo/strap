
#include "string.h"

#include <string.h>
#include <stdlib.h>

String sv(char* string) {
    return (String) { .bytes = string, .size = strlen(string) };
}

String sv_from_bytes(char* bytes, size_t size) {
    return (String) { .bytes = bytes, .size = size };
}

char* sv_to_cstr(String string) {
    char* str = malloc(string.size + 1);
    str[string.size] = 0;
    memcpy(str, string.bytes, string.size);
    return str;
}

String sv_split(String* string, String delim) {
    for (size_t i = 0; i < string->size - delim.size; i++) {
        if (memcmp(string->bytes + i, delim.bytes, delim.size) == 0) {
            String result = { .bytes = string->bytes + i + delim.size,
                              .size  = string->size  - i - delim.size };
            string->size = i;
            return result;
        }
    }
    String result = { .bytes = string->bytes + string->size, .size = 0 };
    return result;
}

