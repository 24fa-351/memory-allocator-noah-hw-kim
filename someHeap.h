#ifndef SOMEHEAP_H
#define SOMEHEAP_H

#include "malloc.h"

unsigned int heap_parent(unsigned int index);
unsigned int heap_left_child(unsigned int index);
unsigned int heap_right_child(unsigned int index);
unsigned int heap_level(unsigned int index);
void heap_swap(MemoryBlock **free_list, int index1, int index2);
void heap_bubble_up(MemoryBlock **free_list, int index);
void heap_bubble_down(MemoryBlock **free_list, int *heap_size, int index);
void heap_insert(MemoryBlock **free_list, int *heap_size, MemoryBlock *ptr);
MemoryBlock *heap_remove(MemoryBlock **free_list, int *heap_size, int idx);

#endif // SOMEHEAP_H