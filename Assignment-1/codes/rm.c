#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file1> [<file2> ...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int c;
    if (argv[1][0] == '-') {
        printf("Do you want to remove file? (1=yes/0=no): ");
        scanf("%d", &c);
        if (c == 1) {
            remove(argv[2]);
        } else {
            printf("File not removed\n");
        }
    } else {
        for (int i = 1; i < argc; i++) {
            if (remove(argv[i]) == 0) {
                printf("Removed file: %s\n", argv[i]);
            } else {
                perror("Error deleting file");
            }
        }
    }
    return 0;
}

