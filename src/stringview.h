
#ifndef STRING_H_
#define STRING_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "arena.h"

typedef struct {
    char* bytes;
    size_t size;
} String;

String sv(char* string);
String sv_from_bytes(char* bytes, size_t size);
#define sv_from_sb(sb) sv_from_bytes(sb->data, sb->size)
#define sv_from_char(c) sv_from_bytes(&c, 1)
char* sv_to_cstr(String string); // TO BE FREED !!!!
String sv_copy(String str, Arena* arena);
String sv_split(String* string, String delim); 
void sv_trim_left(String* string);
void sv_trim_right(String* string);
void sv_trim(String* string);
bool sv_compare(String a, String b);
bool sv_compare_case_ignored(String a, String b);
bool sv_compare_at(String a, String b, size_t index);
bool sv_startswith(String string, String prefix);
bool sv_endswith(String string, String suffix);
char sv_index(String string, size_t index);
int sv_to_int(String string);

#define SV_FMT "%.*s"
#define SvFmt(string) (int) string.size, string.bytes

#endif // STRING_H_
