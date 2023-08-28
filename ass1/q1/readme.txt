For the first part 1)
- The code begins by including necessary header files, such as stdlib.h, stdio.h, unistd.h, and sys/wait.h, which provide the required functions and declarations for process manipulation.

- Within the main() function, we use the fork() system call to create a new process. The fork() call creates a child process that is a copy of the parent process. The parent process receives the child's process ID (PID) as the return value of fork(), while the child process receives a return value of 0.

- In the if (pid == 0) block, we handle the code execution for the child process. The child process prints its own PID using getpid(), which corresponds to statement C. It then prints the parent's PID using getppid(), achieving statement D.

- In the else if (pid > 0) block, we handle the code execution for the parent process. The parent process prints its own PID using getpid(), fulfilling statement A. It then uses the wait(NULL) system call to wait for the child process to complete its execution before continuing. This ensures that the order of statement execution is controlled.

- After the child process completes and the parent process resumes execution, the parent process prints the child's PID (obtained from the pid variable) as specified in statement B.


For the second part 2) 
- The program first creates a child process using the vfork() system call for the calculation of the Fibonacci series. The child process is created using vfork() instead of fork() to optimize memory usage.

- In the if (fib_pid == 0) block, the code inside the child process executes. The execlp() function is used to replace the current process image with the image of the Fibonacci calculation program. The program's path and any necessary arguments are provided as parameters to execlp().

- The parent process (original process) waits for the Fibonacci child process to complete using the wait(NULL) system call. This ensures that the parent process does not continue until the Fibonacci calculation is finished.

- The program then creates another child process using vfork() for the calculation of the factorial.

- Similar to the Fibonacci child process, the factorial child process replaces its image with the image of the factorial calculation program using execlp().

- Finally, the parent process waits for the factorial child process to complete using another wait(NULL) call.

The same is done the bonus part also.