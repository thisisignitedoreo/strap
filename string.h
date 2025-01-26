
#ifndef STRING_H_
#define STRING_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    char* bytes;
    size_t size;
} String;

String sv(char* string);
String sv_from_bytes(char* bytes, size_t size);
char* sv_to_cstr(String string); // TO BE FREED !!!!
String sv_split(String* string, String delim); 
void sv_trim_left(String* string);
void sv_trim_right(String* string);
void sv_trim(String* string);
bool sv_compare(String a, String b);
bool sv_startswith(String string, String prefix);
bool sv_endswith(String string, String suffix);

#define SV_FMT "%.*s"
#define SvFmt(string) (int) string.size, string.bytes

#endif // STRING_H_
