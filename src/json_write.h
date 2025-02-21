
// That is a immideate-mode JSON serializer.
// Basically, ImGui, but for JSON :))

#ifndef JSON_WRITE_H_
#define JSON_WRITE_H_

#include "arena.h"
#include "array.h"

typedef struct {
    StringBuilder* text;
    StringBuilder* path;
} JsonWriteObject;

JsonWriteObject* jsonw_init(Arena* arena);

void jsonw_string(JsonWriteObject* obj, String str);
void jsonw_integer_number(JsonWriteObject* obj, int num);
void jsonw_float_number(JsonWriteObject* obj, float num);
void jsonw_null(JsonWriteObject* obj);
void jsonw_bool(JsonWriteObject* obj, bool b);
void jsonw_true(JsonWriteObject* obj);
void jsonw_false(JsonWriteObject* obj);
void jsonw_dict_start(JsonWriteObject* obj);
void jsonw_dict_end(JsonWriteObject* obj);
void jsonw_dict_key(JsonWriteObject* obj, String string);
void jsonw_list_start(JsonWriteObject* obj);
void jsonw_list_end(JsonWriteObject* obj);

String jsonw_get_string(JsonWriteObject* obj);
// Return text string of the JSON

#endif // JSON_WRITE_H_
