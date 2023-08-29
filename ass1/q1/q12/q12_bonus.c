#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int a = 0, b = 1, c = 1;
void fib(int n) {
    int a = 0;
    int b = 1;
    int c;
    if (n == 1) printf("0");
    if (n == 2) printf("0 1");
    printf("0 1 ");
    for (int i = 0; i < n - 2; i++) {
        c = a + b;
        printf("%d ", c);
        a = b;
        b = c;
    }
    printf("\n");
}
int fact(int n) {
    int ans = 1;
    for (int i = 1; i <= n; i++) {
        ans = ans * i;
    }
    return ans;
}

int main() {
    int fib_pid;
    fib_pid = fork();

    if (fib_pid == 0) {
        // fib child
        execlp("./fact", "hello", (char*)NULL);
        waitpid(getppid(), NULL, 0);
    }
    if (fib_pid > 0) {
        // parent
        printf("fibonacci series upto 16: ");
        fib(16);
     }
}