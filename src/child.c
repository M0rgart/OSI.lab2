#include "common.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *file = fopen(argv[1], "w");
    if (file == NULL) handle_error("fopen");

    float sum = 0.0, value;
    while (scanf("%f", &value) == 1) {
        sum += value;
    }

    fprintf(file, "Sum: %.2f\n", sum);
    fclose(file);

    printf("%.2f\n", sum);
    fflush(stdout);

    return 0;
}