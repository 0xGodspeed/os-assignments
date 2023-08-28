#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    int fib_pid;
    fib_pid = vfork();

    if (fib_pid == 0) {
        // fib child
        int fact_pid;
        fact_pid = vfork();
        if (fact_pid == 0) {
            // fact child
            execlp("./fact", "hello", (char*)NULL);
        }
        if (fact_pid > 0) {
            wait(NULL);
            execlp("./fib", "hello", (char*)NULL);
        }
    }

    if (fib_pid > 0) {
        // parent
        wait(NULL);
        // printf("fibonacci series upto 16: ");
        // fib(16);
        // int fact_pid = vfork();
        // if (fact_pid == 0) {
        //     // fact child
        //     execlp("/home/gs/code/clg/os/ass1/fact", "hello", (char*)NULL);
        // }
        // if (fact_pid > 0) wait(NULL);
    }
}