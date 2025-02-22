
#include "json_read.h"

#include <stdio.h>
#include <ctype.h>

size_t jsonr_traverse_path_separator(Arena* arena, String json, String path, char separator) {
    size_t index = 0;
    if (path.size == 0) return index;
    for (;;) {
        while (isspace(sv_index(json, index))) index++;
        String new_path = sv_split(&path, sv_from_char(separator));
        String key = path;
        path = new_path;
        if (sv_index(json, index) == '{') {
            int seen_open_parens = 0;
            for (;;) {
                if ((sv_index(json, index) == ',' || sv_index(json, index) == '{') && seen_open_parens == 0) {
                    index++;
                    while (sv_index(json, index) != '"') index++;
                    StringBuilder* str = StringBuilder_new(arena);
                    StringBuilder_push(str, '"');
                    for (size_t i = 0; i < key.size; i++) {
                        char ch = key.bytes[i];
                        if (ch >= ' ' && ch <= '~') StringBuilder_push(str, ch);
                        else if (ch == '\n') StringBuilder_push_cstring(str, "\\n");
                        else if (ch == '\r') StringBuilder_push_cstring(str, "\\r");
                        else if (ch == '\t') StringBuilder_push_cstring(str, "\\t");
                        else if (ch == '\b') StringBuilder_push_cstring(str, "\\b");
                        else if (ch == '\f') StringBuilder_push_cstring(str, "\\f");
                        else if (ch == '\"') StringBuilder_push_cstring(str, "\\\"");
                        else if (ch == '\\') StringBuilder_push_cstring(str, "\\\\");
                        else StringBuilder_printf(str, "\\u%04X", ch);
                    }
                    StringBuilder_push(str, '"');
                    String s = sv_from_sb(str);
                    if (sv_compare_at(json, s, index)) {
                        index += s.size;
                        while (sv_index(json, index) != ':') index++;
                        index++;
                        while (isspace(sv_index(json, index))) index++;
                        break;
                    }
                }
                index++;
                if (sv_index(json, index) == '[') seen_open_parens++;
                if (sv_index(json, index) == '{') seen_open_parens++;
                if (sv_index(json, index) == '}') seen_open_parens--;
                if (sv_index(json, index) == ']') seen_open_parens--;
                if (seen_open_parens < 0) return index; // out of bounds
            }
        } else if (sv_index(json, index) == '[') {
            int list_item = sv_to_int(key);
            int seen_commas = 0, seen_open_parens = 0;
            while (seen_commas < list_item) {
                index++;
                if (sv_index(json, index) == ',' && seen_open_parens == 0) seen_commas++;
                if (sv_index(json, index) == '[') seen_open_parens++;
                if (sv_index(json, index) == '{') seen_open_parens++;
                if (sv_index(json, index) == '}') seen_open_parens--;
                if (sv_index(json, index) == ']') seen_open_parens--;
                if (seen_open_parens < 0) return index; // out of bounds
            } index++;
        } else break; // why would we index into an integer or a string?
        if (path.size == 0) break;
    }
    return index;
}

int jsonr_read_int_separator(Arena* arena, String json, String path, char separator) {
    String string = jsonr_read_json_separator(arena, json, path, separator);
    return sv_to_int(string);
}

float jsonr_read_float_separator(Arena* arena, String json, String path, char separator) {
    String string = jsonr_read_json_separator(arena, json, path, separator);
    float number = 0.0f;
    sscanf(string.bytes, "%f", &number);
    return number;
}

String jsonr_read_string_separator(Arena* arena, String json, String path, char separator) {
    String string = jsonr_read_json_separator(arena, json, path, separator);
    if (sv_index(json, 0) != '"') return sv_from_bytes(string.bytes, 0);
    StringBuilder* sb = StringBuilder_new(arena);
    for (size_t index = 1; index < string.size-1; index++) {
        char ch = sv_index(json, index);
        if (ch == '"') break;
        else if (ch == '\\') {
            index++;
            ch = sv_index(json, index);
            if (ch == 'r') StringBuilder_push(sb, '\r');
            else if (ch == 'n') StringBuilder_push(sb, '\n');
            else if (ch == 't') StringBuilder_push(sb, '\t');
            else if (ch == 'b') StringBuilder_push(sb, '\b');
            else if (ch == 'f') StringBuilder_push(sb, '\f');
            else if (ch == '"') StringBuilder_push(sb, '"');
            else if (ch == '\\') StringBuilder_push(sb, '\\');
            else if (ch == 'u') {
                String hex = sv_from_bytes(json.bytes + index + 1, 4);
                index += 4;
                unsigned int s = 0;
                sscanf(hex.bytes, "%04X", &s);
                StringBuilder_push(sb, (char) s);
            }
        } else StringBuilder_push(sb, ch);
    }
    return sv_from_sb(sb);
}

bool jsonr_read_bool_separator(Arena* arena, String json, String path, char separator) {
    String b = jsonr_read_json_separator(arena, json, path, separator);
    return sv_compare(b, sv("true"));
}

bool jsonr_read_null_separator(Arena* arena, String json, String path, char separator) {
    String b = jsonr_read_json_separator(arena, json, path, separator);
    return sv_compare(b, sv("null"));
}

StringArray* jsonr_read_keys_separator(Arena* arena, String json, String path, char separator) {
    String dict = jsonr_read_json_separator(arena, json, path, separator);
    if (sv_index(dict, 0) != '{') return NULL;
    size_t index = 0;
    StringArray* array = StringArray_new(arena);
    do {
        index++;
        while (isspace(sv_index(dict, index))) index++;
        StringBuilder* sb = StringBuilder_new(arena);
        for (;;) {
            index++;
            char ch = sv_index(dict, index);
            if (ch == 0) break; // EOI
            if (ch == '"') break;
            else if (ch == '\\') {
                index++;
                ch = sv_index(dict, index);
                if (ch == 'r') StringBuilder_push(sb, '\r');
                else if (ch == 'n') StringBuilder_push(sb, '\n');
                else if (ch == 't') StringBuilder_push(sb, '\t');
                else if (ch == 'b') StringBuilder_push(sb, '\b');
                else if (ch == 'f') StringBuilder_push(sb, '\f');
                else if (ch == '"') StringBuilder_push(sb, '"');
                else if (ch == '\\') StringBuilder_push(sb, '\\');
                else if (ch == 'u') {
                    String hex = sv_from_bytes(dict.bytes + index + 1, 4);
                    index += 4;
                    unsigned int s = 0;
                    sscanf(hex.bytes, "%04X", &s);
                    StringBuilder_push(sb, (char) s);
                }
            } else StringBuilder_push(sb, ch);
        }
        String key = sv_from_sb(sb);
        StringArray_push(array, key);
        while (sv_index(dict, index) != ':') index++;
        index++;
        while (isspace(sv_index(dict, index))) index++;
        int open_parens = 0;
        for (;;) {
            if (sv_index(dict, index) == 0) break;
            if (sv_index(dict, index) == '[' || sv_index(dict, index) == '{') open_parens++;
            if (sv_index(dict, index) == ']' || sv_index(dict, index) == '}') open_parens--;
            if (sv_index(dict, index) == ',' && open_parens == 0) break;
            if (open_parens < 0) return array;
            index++;
        }
    } while (sv_index(dict, index) == ',');
    return array;
}

size_t jsonr_read_size_separator(Arena* arena, String json, String path, char separator) {
    String dict = jsonr_read_json_separator(arena, json, path, separator);
    if (!(sv_index(dict, 0) == '{' || sv_index(dict, 0) == '[')) return 0;
    size_t index = 1;
    while (isspace(sv_index(dict, index))) index++;
    if (sv_index(dict, index) == '}') return 0;
    size_t size = 1;
    int open_parens = 0;
    for (;;) {
        if (sv_index(dict, index) == 0) break;
        if (sv_index(dict, index) == '[' || sv_index(dict, index) == '{') open_parens++;
        if (sv_index(dict, index) == ']' || sv_index(dict, index) == '}') open_parens--;
        if (sv_index(dict, index) == ',' && open_parens == 0) size++;
        if (open_parens < 0) break;
        index++;
    }
    return size;
}

JsonType jsonr_read_type_separator(Arena* arena, String json, String path, char separator) {
    String obj = jsonr_read_json_separator(arena, json, path, separator);
    if (sv_compare(obj, sv("true"))) return jsonr_bool;
    if (sv_compare(obj, sv("false"))) return jsonr_bool;
    if (sv_compare(obj, sv("null"))) return jsonr_null;
    if (sv_index(obj, 0) == '"') return jsonr_string;
    if (sv_index(obj, 0) == '-' || isdigit(sv_index(obj, 0))) return jsonr_number;
    if (sv_index(obj, 0) == '{') return jsonr_dict;
    if (sv_index(obj, 0) == '[') return jsonr_list;
    return jsonr_null;
}

String jsonr_read_json_separator(Arena* arena, String json, String path, char separator) {
    size_t index = jsonr_traverse_path_separator(arena, json, path, separator);
    size_t start_index = index;
    if (sv_index(json, index) == '{' || sv_index(json, index) == '[') {
        int seen_open_parens = 0;
        do {
            if (sv_index(json, index) == '{' || sv_index(json, index) == '[') seen_open_parens++;
            if (sv_index(json, index) == '}' || sv_index(json, index) == ']') seen_open_parens--;
            index++;
        } while (seen_open_parens > 0);
    } else if (sv_index(json, index) == '-' || isdigit(sv_index(json, index))) {
        do index++;
        while (isdigit(sv_index(json, index)) || isalpha(sv_index(json, index)) || sv_index(json, index) == '.');
    } else if (sv_index(json, index) == '"') {
        do index++;
        while (sv_index(json, index) != '"' || sv_index(json, index-1) == '\\');
        index++;
    } else if (sv_compare_at(json, sv("true"), index)) { index += 4; }
    else if (sv_compare_at(json, sv("false"), index)) { index += 5; }
    else if (sv_compare_at(json, sv("null"), index)) { index += 4; }
    
    return sv_from_bytes(json.bytes + start_index, index - start_index);
}

size_t jsonr_traverse_path(Arena* arena, String json, String path) { return jsonr_traverse_path_separator(arena, json, path, ','); }
int jsonr_read_int(Arena* arena, String json, String path) { return jsonr_read_int_separator(arena, json, path, ','); }
float jsonr_read_float(Arena* arena, String json, String path) { return jsonr_read_float_separator(arena, json, path, ','); }
String jsonr_read_string(Arena* arena, String json, String path) { return jsonr_read_string_separator(arena, json, path, ','); }
bool jsonr_read_bool(Arena* arena, String json, String path) { return jsonr_read_bool_separator(arena, json, path, ','); }
bool jsonr_read_null(Arena* arena, String json, String path) { return jsonr_read_null_separator(arena, json, path, ','); }
StringArray* jsonr_read_keys(Arena* arena, String json, String path) { return jsonr_read_keys_separator(arena, json, path, ','); }
size_t jsonr_read_size(Arena* arena, String json, String path) { return jsonr_read_size_separator(arena, json, path, ','); }
JsonType jsonr_read_type(Arena* arena, String json, String path) { return jsonr_read_type_separator(arena, json, path, ','); }
String jsonr_read_json(Arena* arena, String json, String path) { return jsonr_read_json_separator(arena, json, path, ','); }
