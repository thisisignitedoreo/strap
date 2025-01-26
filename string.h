
#ifndef STRING_H_
#define STRING_H_

#include <stdint.h>

typedef struct {
    char* bytes;
    size_t size;
} String;

String sv(char* string);
String sv_from_bytes(char* bytes, size_t size);
char* sv_to_cstr(String string); // TO BE FREED !!!!
String sv_split(String* string, String delim);

#define SV_FMT "%.*s"
#define SvFmt(string) (int) string.size, string.bytes

#endif // STRING_H_
