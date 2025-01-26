
#include "fileio.h"

#include <stdio.h>
#include <stdlib.h>

bool file_exists(String filename) {
    char* cstr = sv_to_cstr(filename);
    FILE* f = fopen(cstr, "rb");
    free(cstr);
    if (f == NULL) return false;
    fclose(f);
    return true;
}

size_t file_size(String filename) {
    char* cstr = sv_to_cstr(filename);
    FILE* f = fopen(cstr, "rb");
    free(cstr);
    if (f == NULL) return 0;
    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    fclose(f);
    return size;
}

String file_read(String filename, Arena* arena) {
    size_t size = file_size(filename);
    char* buffer = arena_allocate(arena, size);
    if (buffer == NULL) return (String) { .bytes = NULL, .size = 0 };
    file_read_buffer(filename, buffer);
    return (String) { .bytes = buffer, .size = size };
}

void file_read_buffer(String filename, char* buffer) {
    char* cstr = sv_to_cstr(filename);
    FILE* f = fopen(cstr, "rb");
    free(cstr);
    if (f == NULL) return;
    size_t size = file_size(filename);
    fread(buffer, 1, size, f);
    fclose(f);
}

void file_write(String filename, String content) {
    char* cstr = sv_to_cstr(filename);
    FILE* f = fopen(cstr, "wb");
    free(cstr);
    if (f == NULL) return;
    fwrite(content.bytes, 1, content.size, f);
    fclose(f);
}

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

bool dir_exists(String path) {
    char* cstr = sv_to_cstr(path);
    DWORD attr = GetFileAttributesA(cstr);
    free(cstr);
    if (attr == INVALID_FILE_ATTRIBUTES) return false;
    return attr & FILE_ATTRIBUTE_DIRECTORY;
}

StringArray* dir_list(String path, Arena* arena) {
    char* str = arena_allocate(arena, path.size + 3);
    if (str == NULL) return NULL;
    memcpy(str, path.bytes, path.size);
    str[path.size] = '\\';
    str[path.size+1] = '*';
    str[path.size+2] = 0;
    WIN32_FIND_DATA data;
    HANDLE hFind = FindFirstFile(str, &data);
    StringArray* array = StringArray_new(arena);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            size_t len = strlen(data.cFileName);
            char* bytes = arena_allocate(arena, len);
            memcpy(bytes, data.cFileName, len);
            String str = { .bytes = bytes, .size = len };
            StringArray_push(array, str);
        } while (FindNextFile(hFind, &data));
        FindClose(hFind);
    }
    return array;
}

#else

#error "posix isn't supported for now :^("

#endif

