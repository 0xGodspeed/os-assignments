/*
All the main functions with respect to the MeMS are inplemented here
read the function discription for more details

NOTE: DO NOT CHANGE THE NAME OR SIGNATURE OF FUNCTIONS ALREADY PROVIDED
you are only allowed to implement the functions
you can also make additional helper functions a you wish

REFER DOCUMENTATION FOR MORE DETAILS ON FUNSTIONS AND THEIR FUNCTIONALITY
*/
// add other headers as required
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

/*
Use this macro where ever you need PAGE_SIZE.
As PAGESIZE can differ system to system we should have flexibility to modify
this macro to make the output of all system same and conduct a fair evaluation.
*/
#define PAGE_SIZE 4096
#define HOLE 0
#define PROCESS 1

struct main_node {
    int num_of_pages;
    void* p_addr;
    void* v_addr_start;
    void* v_addr_end;
    struct main_node* next;
    struct main_node* prev;
    struct sub_node* sub_head;
    int padding[2]; // to make it 64 bytes (makes calculations easy)
};

struct sub_node {
    int type;
    int size;
    void* p_addr;
    void* v_addr_start;
    void* v_addr_end;
    struct sub_node* next;
    struct sub_node* prev;
    int padding[4]; // to make it 64 bytes (makes calculations easy)
};

void* main_node_tracker;
void* sub_node_tracker;
void* current_main_node_map;
void* current_sub_node_map;

void init_free_list() {
    main_node_tracker = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE,
                             MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    sub_node_tracker = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE,
                            MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    current_main_node_map = main_node_tracker;
    current_sub_node_map = sub_node_tracker;
}

struct main_node* add_main_node() {
    // if no more nodes can be added to the current mmap page
    if (main_node_tracker + sizeof(struct main_node) > current_main_node_map + PAGE_SIZE) {
        current_main_node_map = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE,
                                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        main_node_tracker = current_main_node_map;
        return (struct main_node*)current_main_node_map;
    // else use the current mmap page
    } else {
        struct main_node* new_main_node = (struct main_node*)main_node_tracker;
        main_node_tracker = main_node_tracker + sizeof(struct main_node);
        return new_main_node;
    }
}

struct sub_node* add_sub_node() {
    if (sub_node_tracker + sizeof(struct sub_node) >
        current_sub_node_map + PAGE_SIZE) {
        current_sub_node_map = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE,
                                    MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        sub_node_tracker = current_sub_node_map;
        return (struct sub_node*)current_sub_node_map;
    } else {
        struct sub_node* new_main_node = (struct sub_node*)sub_node_tracker;
        sub_node_tracker = sub_node_tracker + sizeof(struct sub_node);
        return new_main_node;
    }
}

struct main_node* head_main = NULL;
void* start_virtual_address = NULL;

// int main() {
//     main_node_tracker = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE,
//                              MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
//     struct main_node* node1 = add_main_node();
//     struct main_node* node2 = add_main_node();
//     node1->num_of_pages = 1;
//     node2->num_of_pages = 2;
// }

/*
Initializes all the required parameters for the MeMS system. The main parameters
to be initialized are:
1. the head of the free list i.e. the pointer that points to the head of the
free list
2. the starting MeMS virtual p_addr from which the heap in our MeMS virtual
p_addr space will start.
3. any other global variable that you want for the MeMS implementation can be
initialized here. Input Parameter: Nothing Returns: Nothing
*/
void mems_init() {
    init_free_list();
    head_main = add_main_node();

    head_main->num_of_pages = 0;
    head_main->next = head_main;
    head_main->prev = head_main;
    head_main->sub_head = NULL;
    start_virtual_address = (void *)1000;
    head_main->v_addr_start = start_virtual_address;
    head_main->v_addr_end = start_virtual_address;
}

/*
This function will be called at the end of the MeMS system and its main job is
to unmap the allocated memory using the munmap system call. Input Parameter:
Nothing Returns: Nothing
*/
void mems_finish() {}

/*
Allocates memory of the specified size by reusing a segment from the free list
if a sufficiently large segment is available.

Else, uses the mmap system call to allocate more memory on the heap and updates
the free list accordingly.

Note that while mapping using mmap do not forget to reuse the unused space from
mapping by adding it to the free list. Parameter: The size of the memory the
user program wants Returns: MeMS Virtual p_addr (that is created by MeMS)
*/
void* mems_malloc(size_t size) {

// TODO
// size = 0 edge case

    struct main_node* current_main_node = head_main->next;
    // traverseing through main nodes
    while (current_main_node != head_main) {
        struct sub_node* current_sub_node = current_main_node->sub_head;

        // traverseing through sub nodes
        while (current_sub_node != NULL) {
            // if HOLE found and size is sufficient
            if (current_sub_node->type == HOLE && current_sub_node->size >= size) {
                // if size is greater than required, make new hole as well
                if (current_sub_node->size > size) {
                    struct sub_node* new_sub_node = add_sub_node();
                    // struct sub_node* new_sub_node =
                        // (struct sub_node*)malloc(sizeof(struct sub_node));

                    // new hole
                    new_sub_node->type = HOLE;
                    new_sub_node->size = current_sub_node->size - (int)size;
                    new_sub_node->p_addr = (void*)(current_sub_node->p_addr + size);
                    new_sub_node->v_addr_start = (void*)(current_sub_node->v_addr_start + size);
                    new_sub_node->v_addr_end = current_sub_node->v_addr_end;
                    new_sub_node->next = current_sub_node->next;
                    new_sub_node->prev = current_sub_node;
                    if (current_sub_node->next != NULL) {
                        current_sub_node->next->prev = new_sub_node;
                    }
                    current_sub_node->next = new_sub_node;
                    current_sub_node->size = (int)size;
                    current_sub_node->v_addr_end =
                        (void*)(current_sub_node->v_addr_start + size);
                }
                // if size is equal to required, no need to make new hole
                current_sub_node->type = PROCESS;
                // return
                return current_sub_node->v_addr_start;
            }
            current_sub_node = current_sub_node->next;
        }
        current_main_node = current_main_node->next;
    }
    // no hole found :(
    current_main_node = current_main_node->prev;
    int num_of_pages = ceil((double)size / (double)PAGE_SIZE);

    void* p_addr = mmap(NULL, num_of_pages * PAGE_SIZE, PROT_READ | PROT_WRITE,
                        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    // make a new main_node
    struct main_node* new_main_node = add_main_node();
    // struct main_node* new_main_node =
        // (struct main_node*)malloc(sizeof(struct main_node));
    new_main_node->p_addr = p_addr;
    new_main_node->num_of_pages = num_of_pages;
    new_main_node->v_addr_start = current_main_node->v_addr_end;
    // void* temp = current_main_node->v_addr_end + size;
    new_main_node->v_addr_end = current_main_node->v_addr_end + num_of_pages * PAGE_SIZE;
    new_main_node->next = head_main;
    new_main_node->prev = current_main_node;
    current_main_node->next = new_main_node;
    head_main->prev->next = new_main_node;
    head_main->prev = new_main_node;

    start_virtual_address = (void*)(start_virtual_address + size);


    // make the PROCESS sub_node
    struct sub_node* new_sub_node = add_sub_node();
    // struct sub_node* new_sub_node =
        // (struct sub_node*)malloc(sizeof(struct sub_node));
    new_sub_node->type = PROCESS;
    new_sub_node->size = (int)size;
    new_sub_node->p_addr = p_addr;
    new_sub_node->v_addr_start = new_main_node->v_addr_start;
    new_sub_node->v_addr_end = (void*)(new_sub_node->v_addr_start + size);
    new_sub_node->next = current_main_node->sub_head;
    new_sub_node->prev = new_sub_node;

    // to handle edge case when size is exactly the size of the page
    if (size != num_of_pages * PAGE_SIZE) {
        // make remaining HOLE sub_node
        struct sub_node* new_sub_node_hole = add_sub_node();
        // struct sub_node* new_sub_node_hole =
            // (struct sub_node*)malloc(sizeof(struct sub_node));
        new_sub_node_hole->type = HOLE;
        new_sub_node_hole->size = num_of_pages * PAGE_SIZE - (int)size;
        new_sub_node_hole->p_addr = (void*)(p_addr + size);
        new_sub_node_hole->v_addr_start = (void*)(new_sub_node->v_addr_start + size);
        new_sub_node_hole->v_addr_end = (void*)(new_sub_node_hole->v_addr_start + new_sub_node_hole->size);
        new_sub_node_hole->next = NULL;
        new_sub_node_hole->prev = new_sub_node;
        new_sub_node->next = new_sub_node_hole;
    }

    new_main_node->sub_head = new_sub_node;
    return new_sub_node->v_addr_start;
}

/*
this function print the stats of the MeMS system like
1. How many pages are utilised by using the mems_malloc
2. how much memory is unused i.e. the memory that is in freelist and is not
used.
3. It also prints details about each node in the main chain and each segment
(PROCESS or HOLE) in the sub-chain. Parameter: Nothing Returns: Nothing but
should print the necessary information on STDOUT
*/
void mems_print_stats() {}

/*
Returns the MeMS physical p_addr mapped to ptr ( ptr is MeMS virtual p_addr).
Parameter: MeMS Virtual p_addr (that is created by MeMS)
Returns: MeMS physical p_addr mapped to the passed ptr (MeMS virtual p_addr).
*/
void* mems_get(void* v_ptr) {}

/*
this function free up the memory pointed by our virtual_address and add it to
the free list Parameter: MeMS Virtual p_addr (that is created by MeMS) Returns:
nothing
*/
void mems_free(void* v_ptr) {}