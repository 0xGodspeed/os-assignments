#include <stdbool.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define RESET "\x1B[0m"

#define IF_VERBOSE if (verbose)

void dir(char * dir, bool removeDir, bool verbose) {
    if (removeDir) {
        int rc = remove(dir);
        if (rc == 1 && verbose) {
            printf("removed %s", dir);
        }
    }
    int mkdir_rc = mkdir(dir, 770);
    if (mkdir_rc == 0 && verbose) {
        printf("created %s", dir);
    }
    else if (mkdir_rc != 0) {
        printf(RED"[X] ERROR:" RESET "failed to create %s", dir);
    }
}

int main(int argc, char *argv[]) {
    bool remove = false;
    bool verbose = false;
    char* dir;
    if (argc < 2) {
        printf(RED "[-] Usage:" RESET " dir [-option] [dir_name]");
        return EXIT_FAILURE;
    }
    for (int i = 1; i < argc; i++) {
        if (strcmp("-r", argv[i]) == 0) {
            remove = true;
        }
        if (strcmp("-v", argv[i]) == 0) {
            verbose = true;
        }
        else {
            dir = argv[i];   
        }
    }


}