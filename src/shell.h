
#ifndef SHELL_H_
#define SHELL_H_

#include "arena.h"
#include "stringview.h"
#include "array.h"

#ifdef _WIN32
 #include <Windows.h>
#else
 #include <unistd.h>
 #include <sys/types.h>
#endif

#ifdef _WIN32
 typedef HANDLE proc_t;
 #define SHELL_INVALID_PROC INVALID_HANDLE_VALUE
#else
 typedef pid_t proc_t;
 #define SHELL_INVALID_PROC (-1)
#endif
#define SHELL_INVALID_EXITCODE INT_MAX
typedef int exitcode_t;

StringBuilder* shell_render_command(Arena* arena, String program_path, StringArray* arguments);
exitcode_t shell_wait_program(proc_t proc);
bool shell_is_program_running(proc_t proc);
exitcode_t shell_run_program(Arena* arena, String program_path, StringArray* arguments);
proc_t shell_run_program_async(Arena* arena, String program_path, StringArray* arguments);

#endif // SHELL_H_
