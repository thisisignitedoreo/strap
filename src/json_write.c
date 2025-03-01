
#include "json_write.h"

#include "arena.h"

JsonWriteObject* jsonw_init(Arena* arena) {
    JsonWriteObject* obj = arena_malloc(arena, sizeof(JsonWriteObject));
    obj->text = array_new(StringBuilder, arena);
    obj->path = array_new(StringBuilder, arena);
    return obj;
}

void _jsonw_push_comma(JsonWriteObject* obj) {
    if (obj->path->size != 0) *array_push(obj->text) = ',';
}

void jsonw_string(JsonWriteObject* obj, String str) {
    // TODO: support unicode.
    *array_push(obj->text) = '"';
    for (size_t i = 0; i < str.size; i++) {
        char ch = str.bytes[i];
        if (ch >= ' ' && ch <= '~') *array_push(obj->text) = ch;
        else if (ch == '\n') array_sb_push_cstring(obj->text, "\\n");
        else if (ch == '\r') array_sb_push_cstring(obj->text, "\\r");
        else if (ch == '\t') array_sb_push_cstring(obj->text, "\\t");
        else if (ch == '\b') array_sb_push_cstring(obj->text, "\\b");
        else if (ch == '\f') array_sb_push_cstring(obj->text, "\\f");
        else if (ch == '\"') array_sb_push_cstring(obj->text, "\\\"");
        else if (ch == '\\') array_sb_push_cstring(obj->text, "\\\\");
        else array_sb_printf(obj->text, "\\u%04X", ch);
    }
    *array_push(obj->text) = '"';
    _jsonw_push_comma(obj);
}

void jsonw_integer_number(JsonWriteObject* obj, int num) { array_sb_printf(obj->text, "%d", num); _jsonw_push_comma(obj); }
void jsonw_float_number(JsonWriteObject* obj, float num) { array_sb_printf(obj->text, "%f", num); _jsonw_push_comma(obj); }
void jsonw_null(JsonWriteObject* obj) { array_sb_push_cstring(obj->text, "null"); _jsonw_push_comma(obj); }
void jsonw_bool(JsonWriteObject* obj, bool b) { if (b) jsonw_true(obj); else jsonw_false(obj); }
void jsonw_true(JsonWriteObject* obj) { array_sb_push_cstring(obj->text, "true"); _jsonw_push_comma(obj); }
void jsonw_false(JsonWriteObject* obj) { array_sb_push_cstring(obj->text, "false"); _jsonw_push_comma(obj); }

void jsonw_dict_start(JsonWriteObject* obj) {
    *array_push(obj->text) = '{';
    *array_push(obj->path) = '{';
}

void jsonw_dict_end(JsonWriteObject* obj) {
    if (array_get(obj->path, obj->path->size-1) != '{') return; // oh well.
    (void)array_pop(obj->path);
    
    if (array_get(obj->text, obj->text->size-1) == ',') (void)array_pop(obj->text);
    *array_push(obj->text) = '}';
    _jsonw_push_comma(obj);
}

void jsonw_dict_key(JsonWriteObject* obj, String string) {
    jsonw_string(obj, string);
    if (array_get(obj->text, obj->text->size-1) == ',') (void)array_pop(obj->text);
    *array_push(obj->text) = ':';
}

void jsonw_list_start(JsonWriteObject* obj) {
    *array_push(obj->text) = '[';
    *array_push(obj->path) = '[';
}

void jsonw_list_end(JsonWriteObject* obj) {
    if (array_get(obj->path, obj->path->size-1) != '[') return; // oh well.
    (void)array_pop(obj->path);
    
    if (array_get(obj->text, obj->text->size-1) == ',') (void)array_pop(obj->text);
    *array_push(obj->text) = ']';
    _jsonw_push_comma(obj);
}

String jsonw_get_string(JsonWriteObject* obj) {
    return sv_from_sb(obj->text);
}

