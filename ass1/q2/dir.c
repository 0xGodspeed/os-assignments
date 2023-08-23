#define _XOPEN_SOURCE 500

#include <stdbool.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <ftw.h>

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define RESET "\x1B[0m"

void rmrf(char* path, const struct stat* stat, int type, struct FTW* ftw, bool verbose) {
    remove(path);
}

void makeDir(char * dir, bool removeDir, bool verbose) {
    if (removeDir) {
        int rc = nftw(dir, rmrf, 64, FTW_DEPTH | FTW_PHYS);

        if (rc == 0 && verbose) {
            printf(CYN "[-] Removed %s\n" RESET, dir);
        }
    }

    int mkdir_rc = mkdir(dir, 0770);
    if (mkdir_rc == 0 && verbose) {
        printf(CYN "[-] Created %s\n" RESET, dir);
    }
    else if (mkdir_rc != 0) {
        printf(RED"[x] ERROR: " RESET "failed to create %s\n", dir);
        if (verbose && errno == EEXIST) {
            printf(RED "[x] ERROR: " RESET "%s already exists", dir);
        }
    }
}

int main(int argc, char *argv[]) {
    bool remove = false;
    bool verbose = false;
    char* dir;
    if (argc < 2) {
        printf(MAG "[+] Usage:" RESET " dir [-option] [dir_name]");
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
    makeDir(dir, remove, verbose);
    return EXIT_SUCCESS;
}