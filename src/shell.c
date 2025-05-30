
#include "shell.h"
#include <stdlib.h>
#include <limits.h>

#ifndef _WIN32
#include <signal.h>
#include <sys/wait.h>
#endif

StringBuilder* shell_render_command(Arena* arena, String program_path, StringArray* arguments) {
    StringBuilder* sb = array_new(arena);
    bool enquote = memchr(program_path.bytes, ' ', program_path.size);
    if (enquote) *array_push(sb) = '"';
    array_sb_push_string(sb, program_path);
    if (enquote) *array_push(sb) = '"';
    *array_push(sb) = ' ';
    if (arguments) {
        array_foreach(arguments, i) {
            String str = array_get(arguments, i);
            bool enquote = memchr(str.bytes, ' ', str.size);
            if (enquote) *array_push(sb) = '"';
            array_sb_push_string(sb, str);
            if (enquote) *array_push(sb) = '"';
            *array_push(sb) = ' ';
        }
    }
    return sb;
}

exitcode_t shell_wait_program(proc_t proc) {
    if (proc == SHELL_INVALID_PROC) return SHELL_INVALID_EXITCODE;
#ifdef _WIN32
    DWORD result = WaitForSingleObject(proc, INFINITE);
    if (result == WAIT_FAILED) return SHELL_INVALID_EXITCODE;
    DWORD exitcode;
    if (!GetExitCodeProcess(proc, &exitcode)) return SHELL_INVALID_EXITCODE;
    CloseHandle(proc);
    return (exitcode_t) exitcode;
#else
    for (;;) {
        int status = 0;
        if (waitpid(proc, &status, 0) < 0) return SHELL_INVALID_EXITCODE;
        if (WIFEXITED(status)) return WEXITSTATUS(status);
        if (WIFSIGNALED(status)) return SHELL_INVALID_EXITCODE;
    }
#endif
}

bool shell_is_program_running(proc_t proc) {
#ifdef _WIN32
    DWORD exitcode;
    if (!GetExitCodeProcess(proc, &exitcode)) return false;
    return exitcode == STILL_ACTIVE;
#else
    return (bool) kill(proc, 0);
#endif
}

exitcode_t shell_run_program(Arena* arena, String program_path, StringArray* arguments) {
    proc_t proc = shell_run_program_async(arena, program_path, arguments);
    if (proc == SHELL_INVALID_PROC) return SHELL_INVALID_EXITCODE;
    return shell_wait_program(proc);
}

proc_t shell_run_program_async(Arena* arena, String program_path, StringArray* arguments) {
    StringBuilder* sb = shell_render_command(arena, program_path, arguments);
    *array_push(sb) = 0;
#ifdef _WIN32
    STARTUPINFO startInfo;
    ZeroMemory(&startInfo, sizeof(startInfo));
    startInfo.cb = sizeof(STARTUPINFO);
    
    PROCESS_INFORMATION procInfo;
    ZeroMemory(&procInfo, sizeof(PROCESS_INFORMATION));

    BOOL success = CreateProcessA(NULL, sb->data, NULL, NULL, TRUE, 0, NULL, NULL, &startInfo, &procInfo);
    if (!success) return SHELL_INVALID_PROC;

    CloseHandle(procInfo.hThread);
    return procInfo.hProcess;
#else
    pid_t cpid = fork();
    if (cpid < 0) return SHELL_INVALID_PROC;
    if (cpid == 0) {
        // this leaks precious memory
        // womp womp. can not do anything
        char* pn_cstr = sv_to_cstr(program_path);
        CStrArray* vec = array_new(arena);
        *array_push(vec) = pn_cstr;
        array_foreach(arguments, i) {
            String arg = array_get(arguments, i);
            *array_push(vec) = sv_to_cstr(arg);
        }
        *array_push(vec) = NULL;
        if (execvp(pn_cstr, vec->data) < 0) exit(1);
    }
    return cpid;
#endif    
}

