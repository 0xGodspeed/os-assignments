#include "word.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


int bruh() {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        const char* s[] = {"dir", "dir1"};
        execvp("./dir", s);

    } else {
        int status;
        waitpid(pid, &status, 0);
        chdir("./dir1");
        perror("chdir");
        pid_t pid = fork();
        if (pid == 0) {
            const char* s[] = {"dir", "dir2"};
            execvp("/home/gs/code/clg/os/ass1/q2/dir", s);
        }
        else {
            wait(NULL);
        }
    }
}

void fuck() {
    char * command[] = {"test1.txt"};
    int ret = wordRun(2, command);
}
int main() {
    // char command[] = "test";
    fuck();
}