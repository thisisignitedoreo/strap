
#ifndef JSON_READ_H_
#define JSON_READ_H_

#include "stringview.h"
#include "arena.h"
#include "array.h"

// Path is comma-separated list of indexes and keys.
// e.g. "fruits,berries,2" =>
// {"fruits": {"berries": [0, 1, 2]}}
//              Will return THIS ^ value
// "" =>
// "a dangling string..."
// Will return ^ THIS value

typedef enum {
    jsonr_null = 0,
    jsonr_bool,
    jsonr_number,
    jsonr_string,
    jsonr_dict,
    jsonr_list
} JsonType;

size_t jsonr_traverse_path(Arena* arena, String json, String path);
size_t jsonr_traverse_path_separator(Arena* arena, String json, String path, char separator);

int jsonr_read_int(Arena* arena, String json, String path);
float jsonr_read_float(Arena* arena, String json, String path);
String jsonr_read_string(Arena* arena, String json, String path);
bool jsonr_read_bool(Arena* arena, String json, String path);
bool jsonr_read_null(Arena* arena, String json, String path); // true if there is a null value
StringArray* jsonr_read_keys(Arena* arena, String json, String path);
size_t jsonr_read_size(Arena* arena, String json, String path);
JsonType jsonr_read_type(Arena* arena, String json, String path);
String jsonr_read_json(Arena* arena, String json, String path);

int jsonr_read_int_separator(Arena* arena, String json, String path, char separator);
float jsonr_read_float_separator(Arena* arena, String json, String path, char separator);
String jsonr_read_string_separator(Arena* arena, String json, String path, char separator);
bool jsonr_read_bool_separator(Arena* arena, String json, String path, char separator);
bool jsonr_read_null_separator(Arena* arena, String json, String path, char separator);
StringArray* jsonr_read_keys_separator(Arena* arena, String json, String path, char separator);
size_t jsonr_read_size_separator(Arena* arena, String json, String path, char separator);
JsonType jsonr_read_type_separator(Arena* arena, String json, String path, char separator);
String jsonr_read_json_separator(Arena* arena, String json, String path, char separator);

#endif // JSON_READ_H_
