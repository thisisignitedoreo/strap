
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
    char* buffer = arena_malloc(arena, size);
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

#include <windows.h>
#include <shellapi.h>

bool dir_exists(String path) {
    char* cstr = sv_to_cstr(path);
    DWORD attr = GetFileAttributesA(cstr);
    free(cstr);
    if (attr == INVALID_FILE_ATTRIBUTES) return false;
    return attr & FILE_ATTRIBUTE_DIRECTORY;
}

void dir_remove_recursive(String path) {
    char* dir = sv_to_cstr(path);
    SHFILEOPSTRUCTA file_op = {
        NULL,
        FO_DELETE,
        dir,
        "",
        FOF_NOCONFIRMATION |
        FOF_NOERRORUI |
        FOF_SILENT,
        false,
        0,
        "" };
    SHFileOperationA(&file_op);
    free(dir);
}

StringArray* dir_list(String path, Arena* arena) {
    char* str = arena_malloc(arena, path.size + 3);
    if (str == NULL) return NULL;
    memcpy(str, path.bytes, path.size);
    str[path.size] = '\\';
    str[path.size+1] = '*';
    str[path.size+2] = 0;
    WIN32_FIND_DATA data;
    HANDLE hFind = FindFirstFile(str, &data);
    StringArray* array = array_new(StringArray, arena);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            size_t len = strlen(data.cFileName);
            char* bytes = arena_malloc(arena, len);
            memcpy(bytes, data.cFileName, len);
            *array_push(array) = sv_from_bytes(bytes, len);
        } while (FindNextFile(hFind, &data));
        FindClose(hFind);
    }
    return array;
}

StringArray* dir_fnmatch(String pattern, Arena* arena) {
    char* str = sv_to_cstr(pattern);
    WIN32_FIND_DATA data;
    HANDLE hFind = FindFirstFile(str, &data);
    StringArray* array = array_new(StringArray, arena);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            size_t len = strlen(data.cFileName);
            char* bytes = arena_malloc(arena, len);
            memcpy(bytes, data.cFileName, len);
            *array_push(array) = sv_from_bytes(bytes, len);
        } while (FindNextFile(hFind, &data));
        FindClose(hFind);
    }
    free(str);
    return array;
}

void dir_make_directory(String path) {
    char* cstr = sv_to_cstr(path);
    CreateDirectory(cstr, NULL);
    free(cstr);
}

void dir_change_cwd(String path) {
    char* cstr = sv_to_cstr(path);
    SetCurrentDirectory(cstr);
    free(cstr);
}

String dir_get_cwd(Arena* arena) {
    size_t size = GetCurrentDirectory(0, NULL);
    char* cstr = arena_malloc(arena, size);
    GetCurrentDirectory(size, cstr);
    return sv(cstr);
}

#else

#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <glob.h>
#include <ftw.h>

bool dir_exists(String path) {
    char* pathname = sv_to_cstr(path);
    struct stat sb;
    bool isdir = stat(pathname, &sb) == 0 && S_ISDIR(sb.st_mode);
    free(pathname);
    return isdir;
}

int _dir_delete_recursive_helper(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf) {
    (void)sb; (void)typeflag; (void)ftwbuf;
    return remove(fpath);
}

void dir_delete_recursive(String path) {
    char* cstr = sv_to_cstr(path);
    nftw(cstr, _dir_delete_recursive_helper, 64, FTW_DEPTH | FTW_PHYS);
    free(cstr);
}

StringArray* dir_list(String path, Arena* arena) {
    char* pathname = sv_to_cstr(path);
    DIR* dp = opendir(pathname);
    struct dirent* ep;
    StringArray* array = array_new(StringArray, arena);
    if (dp != NULL) {
        while ((ep = readdir(dp)) != NULL) {
            size_t len = strlen(ep->d_name);
            char* bytes = arena_malloc(arena, len);
            memcpy(bytes, ep->d_name, len);
            *array_push(array) = sv_from_bytes(bytes, len);
        }
        (void) closedir(dp);
    }
    free(pathname);
    return array;
}

StringArray* dir_fnmatch(String pattern, Arena* arena) {
    char* cstr = sv_to_cstr(pattern);
    StringArray* sa = array_new(StringArray, arena);
    glob_t pglob;
    if (glob(cstr, 0, NULL, &pglob) != 0) return sa;
    for (size_t i = 0; i < pglob.gl_pathc; i++) {
        String original = sv(pglob.gl_pathv[i]);
        char* copy = arena_malloc(arena, original.size);
        memcpy(copy, original.bytes, original.size);
        *array_push(sa) = sv_from_bytes(copy, original.size);
    }
    globfree(&pglob);
    free(cstr);
    return sa;
}

void dir_make_directory(String path) {
    char* cstr = sv_to_cstr(path);
    if (!dir_exists(path)) mkdir(cstr, 0700);
    free(cstr);
}

void dir_change_cwd(String path) {
    char* cstr = sv_to_cstr(path);
    if (dir_exists(path)) chdir(cstr);
    free(cstr);
}

String dir_get_cwd(Arena* arena) {
    char* cwd = getcwd(NULL, 0);
    if (cwd == NULL) return sv_from_bytes(NULL, 0);
    char* cwda = arena_malloc(arena, strlen(cwd));
    memcpy(cwda, cwd, strlen(cwd));
    String str = sv_from_bytes(cwda, strlen(cwd));
    free(cwd);
    return str;
}

#endif

