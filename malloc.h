#ifndef MALLOC_H
#define MALLOC_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_SIZE 1000
#define MIN_REQUEST_SIZE 1024
#define PADDING_SIZE 8

typedef struct {
   size_t size;
   char* pointer_to_start;

   // int is_free;
   // struct MemoryBlock* next;
   // struct MemoryBlock* prev;
} MemoryBlock;

// typedef struct {
//    MemoryBlock memory_block_arr[MAX_SIZE];
// } FreeList;

void* xmalloc(size_t size);
void* realloc(void* ptr, size_t size);
void* calloc(size_t num, size_t size);

MemoryBlock* find_block_from_free_list(size_t size);
MemoryBlock* get_me_blocks(size_t how_much);
MemoryBlock* break_block(MemoryBlock* memory_block_ptr, size_t size);
void add_block_to_free_list(MemoryBlock* memory_block_ptr);
MemoryBlock remove_block_from_free_list(int idx);

void xfree(void* ptr);
void print_free_list();

#endif