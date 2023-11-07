# MeMS memory system

## Description
MeMS is a memory management system which uses a [free list][1] structure to keep track of free memory blocks. It uses the `mmap()` system call to allocate memory. It is similar to the `malloc()` and `free()` functions in C.

## Assumptions/Conventions
- No `malloc()`, `calloc()`, `realloc()` or `free()` calls are made in the implementation. No arrays are used either. Only `mmap()` and `munmap()` calls are made.
- Default page size is 4096 bytes and can be changed by changing the `PAGE_SIZE` macro in `mems.h`
- Default starting virtual address is 1000 and can be changed by changing the `STARTING_VIRTUAL_ADDRESS` macro in `mems.h`
- PROCESS node is denoted by `1`; HOLD node is denoted by `0`
- `mems_free()` takes in a `void*` pointer to the _start_ of the memory block to be freed (it cannot take in a pointer to the middle of the memory block just like `free()` in C)
- padding has been done to ensure that the size of the node is a multiple of 8 bytes (64 bytes)
- The `MAX_SIZE` macro is used to define the array which holds the addresses of all `mmap()` calls done to make main nodes and sub nodes. This array is used to deallocate the memory for nodes when `mems_finish()` is called.

## Implementation Details
- There are functions `add_main_node()` and `add_sub_node()` which add nodes to the free list. They do _NOT_ allocate a new mmap page for every node; they use the entire mmap page till no more nodes can be added to it. Only when the mmap page is full, a new mmap page is allocated. This is done to save memory.
- `merge_holes()` is called after every `mems_free()` call to merge adjacent holes in the free list. 
- `mems_finish()` deallocates all the mmap pages allocated by the program including the memory allocated to the user as well as the memory used to store the free list.
- Global variables used: 
  - `void* current_main_node_map`: starting pointer of the current map being used to allocate main_nodes
  - `void* current_sub_node_map`: starting pointer of the current map being used to allocate sub_nodes
  - `void* main_node_tracker`: keeps track of the current pointer being used to allocate main_nodes in the current map
  - `void* sub_node_tracker`: keeps track of the current pointer being used to allocate sub_nodes in the current map

[1]: https://en.wikipedia.org/wiki/Free_list