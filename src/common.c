#include "common.h"
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <errno.h>
#endif

void handle_error(const char *msg) {
#ifdef _WIN32
    fprintf(stderr, "%s failed. Error code: %lu\n", msg, GetLastError());
#else
    perror(msg);
#endif
    exit(EXIT_FAILURE);
}