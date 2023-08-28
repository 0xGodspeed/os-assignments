`fork()` is used to spawn a child process and its return value is stored in `pid`.

`pid` is zero for the child process and takes the actual pid of the child process in the parent.

We use this fact to write separate code for child and parent processes.

In the child process, `getppid()` is used to get the pid of the parent process and `getpid()` is used to get the pid of itself.
In the parent process, `pid` variable contains the child process pid while `getpid()` is used to get the pid of itself.
