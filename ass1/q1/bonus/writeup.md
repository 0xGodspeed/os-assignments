In q1 bonus we are supposed to spawn two child processes from a single parent process (q12) using `vfork()`.

The first child process is forked (fib).
Within it, we spawn another child process (fact).

We first let the second child process finish using `wait()` and factorial is printed.

Then the parent (fib) is executed.

Our main program wait for the fib child to execute.
