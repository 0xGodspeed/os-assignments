// parent.c
#define _POSIX_C_SOURCE 199309L
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

// void setSchedulingPolicy(int pid, int policy, int priority) {
//     struct sched_param param;
//     param.sched_priority = priority;

//     if (sched_setscheduler(pid, policy, &param) == -1) {
//         perror("sched_setscheduler");
//         exit(EXIT_FAILURE);
//     }
// }

// int runChildProcess(const char *filename, int scheduling_policy,
//                     FILE *output_file) {
//     int priority;
//     if (scheduling_policy == SCHED_OTHER) {
//         priority = 0;
//     } else {
//         priority = 50;
//     }

//     pid_t pid = fork();

//     if (pid == 0) {
//         // Child process
//         printf("priority: %d pid: %d scheduling policy: %d\n", priority,
//         getpid(), scheduling_policy); setSchedulingPolicy(getpid(),
//         scheduling_policy, priority); execl(filename, filename, NULL);
//         perror("execl");
//         exit(EXIT_FAILURE);
//     } else if (pid < 0) {
//         perror("fork");
//         exit(EXIT_FAILURE);
//     }
//     return pid;
//     // Parent process waits for the child and measures execution time
// }

int main() {
    const int num_processes = 3;
    int child_pids[num_processes];
    FILE *output_file = fopen("execution_times.txt", "w");

    if (output_file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    // Fork all child processes and measure execution times
    int schedules[3] = {SCHED_OTHER, SCHED_FIFO, SCHED_RR};
    int pid;
    struct timespec times[3][2];
    for (int i = 0; i < num_processes; i++) {
        // pid = runChildProcess("./count", schedules[i], output_file);
        int priority;
        if (schedules[i] == SCHED_OTHER) {
            priority = 0;
        } else {
            priority = 50;
        }

        clock_gettime(CLOCK_REALTIME, &times[i][0]);
        pid = fork();

        if (pid == 0) {
            // Child process
            printf("priority: %d pid: %d scheduling policy: %d\n", priority,
                   getpid(), schedules[i]);
            // setSchedulingPolicy(getpid(), schedules[i], priority);
            struct sched_param param;
            param.sched_priority = priority;
            
            if (sched_setscheduler(getpid(), schedules[i], &param) == -1) {
                perror("sched_setscheduler");
                exit(EXIT_FAILURE);
            }
            execvp("./count", NULL);
            // execl("./count", "./count", NULL);
            perror("execl");
            exit(EXIT_FAILURE);
        } else if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        child_pids[i] = pid;
        printf("Process %d (Policy %d) created\n", pid, i);
    }
    // Parent process waits for all child processes to finish
    for (int i = 0; i < num_processes; i++) {
        // int status;

        // waitpid(child_pids[i], &status, 0);
        // struct timespec temp_start;
        int pid, status;
        // pid = wait(NULL);
        pid  = waitpid(-1, &status, 0);
        printf("%d\n", pid);
        // waitpid(pid, &status, 0);
        // Write execution time to the output file
        for (int j = 0; j < 3; j++) {
            if (child_pids[j] == pid) {
                // times[j][0] = temp_start;
                clock_gettime(CLOCK_REALTIME, &times[j][1]);
            }
        }
    }
    for (int j = 0; j < 3; j++) {
        // struct timespec elapsed_time = {
        //     (times[j][1].tv_sec - times[j][0].tv_sec),
        //     (times[j][1].tv_nsec - times[j][0].tv_nsec)};

        long long elapsed_time =
            (times[j][1].tv_sec - times[j][0].tv_sec) * 1000000000 +
            (times[j][1].tv_nsec - times[j][0].tv_nsec);
        // if (elapsed_time.tv_nsec < 0) {
        //     elapsed_time.tv_sec -= 1;
        //     elapsed_time.tv_nsec += 1000000000;
        // }
        fprintf(output_file,
                "Process %d (Policy %d) executed in %lld nanoseconds\n",
                child_pids[j], j, elapsed_time);
        printf("Process %d (Policy %d) executed in %lld nanoseconds\n",
               child_pids[j], schedules[j], elapsed_time);
    }

    // Close the output file
    fclose(output_file);

    return 0;
}
