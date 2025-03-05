
#include "stringview.h"

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

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

void sv_trim_left(String* string) {
    while (string->size > 0 && isspace(string->bytes[0])) string->bytes++;
}

void sv_trim_right(String* string) {
    while (string->size > 0 && isspace(string->bytes[string->size-1])) string->size--;
}

void sv_trim(String* string) {
    sv_trim_left(string);
    sv_trim_right(string);
}

bool sv_compare(String a, String b) {
    if (a.size != b.size) return false;
    return memcmp(a.bytes, b.bytes, a.size) == 0;
}

bool sv_compare_case_ignored(String a, String b) {
    if (a.size != b.size) return false;
    for (size_t i = 0; i < a.size; i++) {
        if (tolower(a.bytes[i]) != tolower(b.bytes[i])) return false;
    }
    return true;
}

bool sv_compare_at(String a, String b, size_t index) {
    if (a.size - index < b.size) return false;
    // printf("SV DEBUG: \"%.*s\" == \""SV_FMT"\"\n", (int) b.size, a.bytes + index, SvFmt(b));
    return memcmp(a.bytes + index, b.bytes, b.size) == 0;
}

bool sv_startswith(String string, String prefix) {
    if (string.size < prefix.size) return false;
    return memcmp(string.bytes, prefix.bytes, prefix.size) == 0;
}

bool sv_endswith(String string, String suffix) {
    if (string.size < suffix.size) return false;
    return memcmp(string.bytes + (string.size - suffix.size), suffix.bytes, suffix.size) == 0;
}

char sv_index(String string, size_t index) {
    if (index >= string.size) return 0;
    return string.bytes[index];
}

int sv_to_int(String string) {
    int mult = 1;
    unsigned base = 0;
    size_t n = 0;
    
    sv_trim_left(&string);
    
    if (sv_index(string, n) == '-') { mult *= -1; n++; }
    else if (!isdigit(sv_index(string, n))) return 0;
    
    while (isdigit(sv_index(string, n))) {
        base = (base*10) + (sv_index(string, n) - '0'); n++;
    }
    
    return base * mult;
}
