#ifndef _WIN32
#include "platform.h"
#include "common.h"
#include <unistd.h>
#include <sys/wait.h>

int create_pipe_pair(pipe_handle_t *read_end, pipe_handle_t *write_end) {
    int fds[2];
    if (pipe(fds) == -1) return -1;
    *read_end = fds[0];
    *write_end = fds[1];
    return 0;
}

process_info_t spawn_child(const char *child_path, const char *filename, pipe_handle_t child_stdin, pipe_handle_t child_stdout) {
    pid_t pid = fork();
    if (pid == -1) handle_error("fork");

    if (pid == 0) {
        if (dup2(child_stdin, STDIN_FILENO) == -1) handle_error("dup2 stdin");
        if (dup2(child_stdout, STDOUT_FILENO) == -1) handle_error("dup2 stdout");
        
        close(child_stdin);
        close(child_stdout);
        
        execl(child_path, child_path, filename, NULL);
        handle_error("execl");
    }
    
    close(child_stdin);
    close(child_stdout);
    return pid;
}

void write_to_pipe(pipe_handle_t pipe, const char *data, size_t len) {
    if (write(pipe, data, len) == -1) handle_error("write");
}

int read_from_pipe(pipe_handle_t pipe, char *buffer, size_t max_len) {
    ssize_t bytes = read(pipe, buffer, max_len - 1);
    if (bytes == -1) return -1;
    buffer[bytes] = '\0';
    return (int)bytes;
}

void wait_for_child(process_info_t proc_info) {
    int status;
    waitpid(proc_info, &status, 0);
}

void close_pipe(pipe_handle_t pipe) {
    close(pipe);
}
#endif