#ifdef _WIN32
#include "platform.h"
#include "common.h"
#include <string.h>

int create_pipe_pair(pipe_handle_t *read_end, pipe_handle_t *write_end) {
    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    HANDLE hRead, hWrite;
    if (!CreatePipe(&hRead, &hWrite, &saAttr, 0)) {
        return -1;
    }
    *read_end = hRead;
    *write_end = hWrite;
    return 0;
}

process_info_t spawn_child(const char *child_path, const char *filename, pipe_handle_t child_stdin, pipe_handle_t child_stdout) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    si.hStdOutput = child_stdout;
    si.hStdInput = child_stdin;
    si.dwFlags |= STARTF_USESTDHANDLES;

    char cmdline[512];
    snprintf(cmdline, sizeof(cmdline), "\"%s\" \"%s\"", child_path, filename);

    ZeroMemory(&pi, sizeof(pi));
    if (!CreateProcess(NULL, cmdline, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        handle_error("CreateProcess");
    }

    CloseHandle(child_stdin);
    CloseHandle(child_stdout);

    return pi;
}

void write_to_pipe(pipe_handle_t pipe, const char *data, size_t len) {
    DWORD written;
    if (!WriteFile(pipe, data, (DWORD)len, &written, NULL)) {
        handle_error("WriteFile");
    }
}

int read_from_pipe(pipe_handle_t pipe, char *buffer, size_t max_len) {
    DWORD bytes_read;
    if (!ReadFile(pipe, buffer, (DWORD)(max_len - 1), &bytes_read, NULL)) {
        return -1;
    }
    buffer[bytes_read] = '\0';
    return (int)bytes_read;
}

void wait_for_child(process_info_t proc_info) {
    WaitForSingleObject(proc_info.hProcess, INFINITE);
    CloseHandle(proc_info.hProcess);
    CloseHandle(proc_info.hThread);
}

void close_pipe(pipe_handle_t pipe) {
    CloseHandle(pipe);
}
#endif