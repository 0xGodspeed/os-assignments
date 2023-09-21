# Linux Scheduling Policies Benchmarking

This code contains a solution for benchmarking Linux scheduling policies for different processes. The goal is to create three processes, each counting from 1 to 2^32, with each process using a different scheduling policy. The execution times of these processes are measured, and the results are saved in an output file. The proccesses run simulatenously.

## Overview

The solution consists of a C program (`schedule.c`) that creates and manages three child processes, each with a different scheduling policy:

1. **Process 1**: Uses `SCHED_OTHER` scheduling discipline with standard priority (`nice:0`).
2. **Process 2**: Uses `SCHED_FIFO` scheduling discipline with default priority.
3. **Process 3**: Uses `SCHED_RR` scheduling discipline with default priority.

The parent process measures the execution times of each child process and records the results in an output file.
The output is written to `execution_times.txt` which is parsed by the python script to generate a plot.
`taskset` has been used to make sure the process only uses a single CPU core while running the processes.
Plots have been made using the python library 'plotly'. 

## How to run
- run using `sudo make run`. 
- to create a plot of the execution times, use `make plot`
