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
    int padding[2];
};

struct sub_node {
    int type;
    int size;
    void* p_addr;
    void* v_addr_start;
    void* v_addr_end;
    struct sub_node* next;
    struct sub_node* prev;
    int padding[4];
};
void* main_node_tracker;
void* sub_node_tracker;
void* current_main_node_map;
void* current_sub_node_map;

struct main_node* add_main_node() {
    if (main_node_tracker + sizeof(struct main_node) >
        current_main_node_map + PAGE_SIZE) {
        current_main_node_map = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE,
                                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        main_node_tracker = current_main_node_map;
        return (struct main_node*)current_main_node_map;
    } else {
        struct main_node* new_main_node = (struct main_node*)main_node_tracker;
        main_node_tracker = main_node_tracker + sizeof(struct main_node);
        return new_main_node;
    }
}

struct main_node* head_main = NULL;
void* start_virtual_address = NULL;

int main() {
    printf("%d\n", sizeof(struct sub_node));
    // main_node_tracker = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE,
    //                          MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    // current_main_node_map = main_node_tracker;
    // struct main_node* bruh[64];
    // for (int i = 0; i < 64; i++) {
    //     bruh[i] = add_main_node();
    //     bruh[i]->num_of_pages = i;
    // }
    // struct main_node* temp = add_main_node();
}
