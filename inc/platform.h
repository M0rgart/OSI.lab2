#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdio.h>
#include <stddef.h>

#ifdef _WIN32
    #include <windows.h>
    typedef HANDLE pipe_handle_t;
    typedef PROCESS_INFORMATION process_info_t;
#else
    typedef int pipe_handle_t;
    typedef int process_info_t;
#endif


int create_pipe_pair(pipe_handle_t *read_end, pipe_handle_t *write_end);
process_info_t spawn_child(const char *child_path, const char *filename, pipe_handle_t child_stdin, pipe_handle_t child_stdout);
void write_to_pipe(pipe_handle_t pipe, const char *data, size_t len);
int read_from_pipe(pipe_handle_t pipe, char *buffer, size_t max_len);
void wait_for_child(process_info_t proc_info);
void close_pipe(pipe_handle_t pipe);

#endif