#include "common.h"
#include "platform.h"
#include <string.h>

int main() {
    pipe_handle_t pipe1_read, pipe1_write;
    pipe_handle_t pipe2_read, pipe2_write;
    char filename[256];
    char input_buf[1024];
    char result_buf[1024];

    if (create_pipe_pair(&pipe1_read, &pipe1_write) != 0) handle_error("pipe1");
    if (create_pipe_pair(&pipe2_read, &pipe2_write) != 0) handle_error("pipe2");

    #ifdef _WIN32
    SetHandleInformation(pipe1_write, HANDLE_FLAG_INHERIT, 0);
    SetHandleInformation(pipe2_read, HANDLE_FLAG_INHERIT, 0);
    #endif

    printf("Enter filename: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) handle_error("fgets");
    filename[strcspn(filename, "\n")] = 0;

    process_info_t child = spawn_child("child.exe", filename, pipe1_read, pipe2_write);

    printf("Enter numbers (space-separated): ");
    fflush(stdout);
    if (fgets(input_buf, sizeof(input_buf), stdin) == NULL) handle_error("fgets numbers");
    
    write_to_pipe(pipe1_write, input_buf, strlen(input_buf));
    close_pipe(pipe1_write);

    int bytes = read_from_pipe(pipe2_read, result_buf, sizeof(result_buf));
    if (bytes > 0) {
        printf("Parent received sum: %s", result_buf);
    } else {
        printf("Parent: read_from_pipe returned %d (no data or EOF)\n", bytes);
    }
    close_pipe(pipe2_read);

    wait_for_child(child);
    printf("Child finished.\n");

    return 0;
}
