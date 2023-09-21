# Kernel Module: Running Processes Counter

## Program Logic Description

This kernel module, "running_processes," implements a custom system call in the Linux kernel. The primary purpose of this module is to count and print the number of currently running processes when it is loaded into the kernel. This README provides an overview of the program logic and functionality.

### Kernel Module Basics

A kernel module is a loadable kernel component that can be dynamically added to or removed from a running Linux kernel. The "running_processes" module is designed to be loaded into the kernel to perform its specific task.

### `custom_init` Function

When the module is loaded into the kernel, the `custom_init` function is called. Here's what happens within this function:

1. **Initialization**: The module initializes by declaring a pointer to a `task_struct` (a structure that represents a process in the kernel) and a variable `process_count` to keep track of the number of running processes.

2. **Locking the Task List**: The module locks the task list using `rcu_read_lock()`. This is necessary to ensure that the task list remains consistent during the enumeration of processes.

3. **Enumeration of Processes**: It then uses a for-each loop to iterate through the task list, where each iteration represents one process. Within this loop, it checks the `__state` field of each process. If the `__state` field is `0`, it considers the process to be running and prints information about the process, including its PID, name, and state. Additionally, it increments the `process_count` for each running process.

4. **Unlocking the Task List**: After the loop has finished, the module unlocks the task list using `rcu_read_unlock()`.

5. **Printing the Process Count**: Finally, the module prints the total count of running processes to the kernel logs using `printk()`. This count is calculated based on the number of running processes encountered during the enumeration.

### `custom_exit` Function

The `custom_exit` function is called when the module is unloaded from the kernel. In this function, the module performs any necessary cleanup or resource release operations. In your code, it simply prints a message to the kernel logs indicating that the module is exiting.

## Usage

To use this kernel module:

1. Ensure that you have the necessary kernel header files installed on your system.

2. Compile the module using the provided `Makefile` with the `make` command.

3. Load the `.ko` module file into the kernel using `insmod`.

4. Check the kernel logs using `dmesg` to see the count of currently running processes.

5. To remove the module, use `rmmod`.

