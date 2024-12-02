#ifndef MALLOC_H
#define MALLOC_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

#define MIN_BLOCK_SIZE 256
#define MAX_HEAP_SIZE 1000
#define ONE_HUNDRED_MB 100 * 1024 * 1024
#define BLOCK_METADATA_SIZE sizeof(MemoryBlock)

typedef struct
{
   size_t size;
   char *ptr_to_start;
} MemoryBlock;

void *xmalloc(size_t size);
void *xrealloc(void *ptr, size_t size);
void *xcalloc(size_t num, size_t size);

MemoryBlock *find_block_from_free_list(size_t size);
MemoryBlock *get_me_blocks(size_t how_much);
MemoryBlock *break_block(MemoryBlock *memory_block_ptr, size_t size);
void add_block_to_free_list(MemoryBlock *memory_block_ptr);
MemoryBlock *remove_block_from_free_list(int idx);
void xfree(void *ptr);
void print_free_list();

// heap structure
void heap_insert(MemoryBlock *ptr);
MemoryBlock *heap_remove(int idx);
void heap_bubble_up(int index);
void heap_bubble_down(int index);
unsigned int heap_parent(unsigned int index);
unsigned int heap_left_child(unsigned int index);
unsigned int heap_right_child(unsigned int index);
void heap_swap(int idx1, int idx2);

#endif