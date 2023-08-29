#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pid = fork();
    if (pid == 0) {
        // child
        printf("parent (P) is having ID %d\n", getppid());
        printf("child is having ID %d\n", getpid());
    }
    else if (pid > 0) {
        // parent
        wait(NULL);
        printf("my parent ID is %d\n", getpid());
        printf("child is having ID %d\n", pid);
    }
}