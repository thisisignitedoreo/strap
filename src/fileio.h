
#ifndef FILEIO_H_
#define FILEIO_H_

#include <stdint.h>
#include <stdbool.h>

#include "arena.h"
#include "stringview.h"
#include "array.h"

bool file_exists(String filename);
size_t file_size(String filename);
String file_read(String filename, Arena* arena);
void file_read_buffer(String filename, char* buffer);
void file_write(String filename, String content);

bool dir_exists(String path);
StringArray* dir_list(String path, Arena* arena);

#endif // FILEIO_H_
