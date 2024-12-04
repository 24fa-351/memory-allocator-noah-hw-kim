#include "someHeap.h"

unsigned int heap_parent(unsigned int index)
{
    if (index == 0)
    {
        return 0;
    }
    return (index - 1) / 2;
}

unsigned int heap_left_child(unsigned int index)
{
    return index * 2 + 1;
}

unsigned int heap_right_child(unsigned int index)
{
    return index * 2 + 2;
}

unsigned int heap_level(unsigned int index)
{
    // base
    if (index == 0)
    {
        return 0;
    }
    // recursive
    return heap_level(heap_parent(index)) + 1;
}

void heap_swap(MemoryBlock **free_list, int index1, int index2)
{
    MemoryBlock *tmp = free_list[index1];
    free_list[index1] = free_list[index2];
    free_list[index2] = tmp;
}

void heap_bubble_up(MemoryBlock **free_list, int index)
{
    int curVal = free_list[index]->size;
    unsigned int parentIdx = heap_parent(index);
    int parentVal = free_list[parentIdx]->size;

    while (index > 0 && parentVal > curVal)
    {
        heap_swap(free_list, parentIdx, index);
        index = parentIdx;
        curVal = free_list[index]->size;
        parentIdx = heap_parent(index);
        parentVal = free_list[parentIdx]->size;
    }
}

void heap_bubble_down(MemoryBlock **free_list, int *heap_size, int index)
{
    while (index < *heap_size)
    {
        unsigned int leftChildIdx = heap_left_child(index);
        unsigned int rightChildIdx = heap_right_child(index);

        unsigned long long curVal = free_list[index]->size;
        unsigned long long leftChildVal = (leftChildIdx < *heap_size) ? free_list[leftChildIdx]->size : ULLONG_MAX;
        unsigned long long rightChildVal = (rightChildIdx < *heap_size) ? free_list[rightChildIdx]->size : ULLONG_MAX;

        if (curVal <= leftChildVal && curVal <= rightChildVal)
        {
            break;
        }

        if (leftChildVal < rightChildVal)
        {
            heap_swap(free_list, index, leftChildIdx);
            index = leftChildIdx;
        }
        else
        {
            heap_swap(free_list, index, rightChildIdx);
            index = rightChildIdx;
        }
    }
}

void heap_insert(MemoryBlock **free_list, int *heap_size, MemoryBlock *ptr)
{
    if (*heap_size >= MAX_HEAP_SIZE)
    {
        fprintf(stderr, "Error: Free list is full!\n");
        return;
    }

    free_list[*heap_size] = ptr;

    heap_bubble_up(free_list, *heap_size);
    *heap_size += 1;
}

MemoryBlock *heap_remove(MemoryBlock **free_list, int *heap_size, int idx)
{
    if (heap_size == 0)
    {
        return NULL;
    }

    MemoryBlock *block = free_list[idx];
    *heap_size -= 1;
    free_list[idx] = free_list[*heap_size];

    // then bubble it down to its correct position
    heap_bubble_down(free_list, heap_size, idx);

    return block;
}