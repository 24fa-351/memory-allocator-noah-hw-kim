#ifndef CUSTOM_MALLOC_H
#define CUSTOM_MALLOC_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_SIZE 1000
#define ONE_GB (1L << 30)

typedef struct {
   size_t size;
   // int is_free;
   // struct MemoryBlock* next;
   // struct MemoryBlock* prev;
} MemoryBlock;

// typedef struct {
//    MemoryBlock memory_block_arr[MAX_SIZE];
// } FreeList;

void* malloc(size_t size);
void* realloc(void* ptr, size_t size);
void* calloc(size_t num, size_t size);

MemoryBlock* find_block_from_free_list(MemoryBlock* free_list_arr_ptr,
                                       size_t size);
MemoryBlock* get_me_blocks(__ssize_t how_much);
MemoryBlock* break_block(MemoryBlock* free_list_ptr,
                         MemoryBlock* memory_block_ptr, size_t size);
void add_block_to_free_list(MemoryBlock* free_list_ptr,
                            MemoryBlock* memory_block_ptr);
MemoryBlock* remove_block_from_free_list(MemoryBlock* free_list_ptr, int idx);

void free(void* ptr);
void print_free_list(void* free_list);

#endif