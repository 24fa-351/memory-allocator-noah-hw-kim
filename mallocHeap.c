#include "malloc.h"
#include <limits.h>
#define MIN_BLOCK_SIZE 256

// keeps track of the free blocks
MemoryBlock *free_list[MAX_SIZE];
int heap_size = 0;

void *xmalloc(size_t size)
{
   if (size == 0)
   {
      return NULL;
   }

   // fprintf(stderr, "Requested size from user: %ld\n", size);

   if (size < MIN_BLOCK_SIZE)
   {
      size = MIN_BLOCK_SIZE;
   }

   MemoryBlock *memory_block_ptr = (MemoryBlock *)find_block_from_free_list(size);
   if (memory_block_ptr == NULL)
   {
      // fprintf(stderr, "Memory block not found in free list.\n");
      memory_block_ptr = get_me_blocks(size);
      // fprintf(stderr, "We got memory block size from system: %ld\n",
      //         memory_block_ptr->size);
   }

   memory_block_ptr = break_block(memory_block_ptr, size);

   if (memory_block_ptr == NULL)
   {
      fprintf(stderr, "Error: Memory block is NULL!\n");
      return NULL;
   }

   fprintf(stderr, "Returning pointer to user: %p\n", memory_block_ptr->ptr_to_start);

   print_free_list(free_list);

   return (void *)(memory_block_ptr->ptr_to_start);
}

void *xrealloc(void *ptr, size_t size)
{
   if (ptr == NULL)
   {
      return xmalloc(size);
   }

   if (size == 0)
   {
      xfree(ptr);
      return NULL;
   }

   MemoryBlock *memory_block_ptr = (MemoryBlock *)((char *)ptr - sizeof(MemoryBlock));

   if (memory_block_ptr->size >= size)
   {
      return ptr;
   }

   void *new_ptr = xmalloc(size);
   if (new_ptr == NULL)
   {
      return NULL;
   }

   memcpy(new_ptr, ptr, memory_block_ptr->size);
   xfree(ptr);

   return new_ptr;
};

void *calloc(size_t num, size_t size);

MemoryBlock *find_block_from_free_list(size_t size)
{
   for (int idx = 0; idx < heap_size; idx++)
   {
      if (free_list[idx]->size >= size)
      {
         fprintf(stderr, "Found block at index: %d, size: %ld, address: %p, address_start: %p\n", idx, free_list[idx]->size, free_list[idx], free_list[idx]->ptr_to_start);
         // fprintf(stderr, "Block size: %ld\n", free_list[idx].size);
         MemoryBlock *removed_block_ptr = heap_remove(idx);
         // MemoryBlock *removed_block_ptr = &removed_block;

         if (removed_block_ptr->size == 0)
         {
            fprintf(stderr, "Error: Removed block size is 0!\n");
            return NULL;
         }

         // fprintf(stderr, "Removed block size: %ld\n",
         // removed_block_ptr->size);
         return removed_block_ptr;
      }
   }
   return NULL;
}

MemoryBlock *get_me_blocks(size_t how_much)
{
   size_t required_size;

   if (how_much > ONE_HUNDRED_MB)
   {
      required_size = (how_much + sizeof(size_t) - 1) & ~(sizeof(size_t) - 1);
   }
   else
   {
      required_size = ONE_HUNDRED_MB;
   }

   void *block_start_ptr = sbrk(required_size + BLOCK_METADATA_SIZE);
   if (block_start_ptr == (void *)-1)
   {
      perror("sbrk failed");
      return NULL;
   }

   MemoryBlock *memory_block_ptr = (MemoryBlock *)block_start_ptr;
   memory_block_ptr->size = required_size;
   memory_block_ptr->ptr_to_start = (char *)block_start_ptr + sizeof(MemoryBlock);

   return memory_block_ptr;
}

// MemoryBlock *remove_block_from_free_list(int idx)
// {
//    if (idx < 0 || idx >= heap_size)
//    {
//       fprintf(stderr, "Index out of bounds.\n");
//       return NULL;
//    }

//    MemoryBlock *removed_memory_block = free_list[idx];
//    // fprintf(stderr, "Removed block size before overwrite array: %ld\n",
//    //         removed_memory_block.size);

//    for (int i = idx; i < heap_size - 1; i++)
//    {
//       free_list[i] = free_list[i + 1];
//    }

//    heap_size--;

//    // fprintf(stderr, "Removed block size after overwrite array: %ld\n",
//    //         removed_memory_block.size);

//    return removed_memory_block;
// }

MemoryBlock *break_block(MemoryBlock *memory_block_ptr, size_t size)
{
   if (memory_block_ptr->size < size)
   {
      fprintf(stderr,
              "Block size is less than required size.\nRequested size: %ld, "
              "Block size: %ld\n",
              size, memory_block_ptr->size);
      return NULL;
   }

   if (memory_block_ptr->size == size)
   {
      return memory_block_ptr;
   }

   char *extra_block_start = (char *)memory_block_ptr + size + sizeof(MemoryBlock);

   MemoryBlock *extra_memory_block_ptr = (MemoryBlock *)extra_block_start;
   extra_memory_block_ptr->size = memory_block_ptr->size - size;
   extra_memory_block_ptr->ptr_to_start = (char *)extra_block_start + sizeof(MemoryBlock);

   printf("Original Memory Block Address: %p\nExtra Memory Block Address: %p\n", memory_block_ptr, extra_memory_block_ptr);

   memory_block_ptr->size = size;

   heap_insert(extra_memory_block_ptr);

   return memory_block_ptr;
}

// void add_block_to_free_list(MemoryBlock *ptr)
// {
//    if (heap_size >= MAX_SIZE)
//    {
//       fprintf(stderr, "Error: Free list is full!\n");
//       return;
//    }
//    // printf("Adding block to free list: %p\n", (void *)ptr);
//    free_list[heap_size] = ptr;
//    // printf("Added block to free list: %p\n", (void *)free_list[cur_idx]);
//    heap_size++;
// }

void xfree(void *ptr)
{
   if (ptr == NULL)
   {
      return;
   }

   MemoryBlock *memory_block_ptr = (MemoryBlock *)((char *)ptr - sizeof(MemoryBlock));

   // printf("Freeing memory block address: %p\n", memory_block_ptr);

   heap_insert(memory_block_ptr);

   print_free_list(free_list);
}

void print_free_list()
{
   printf("\n=== Free List Starts! ===\n");
   for (int idx = 0; idx < heap_size; idx++)
   {
      printf("Block %d: Size - %ld, Address:%p, Address_start: %p\n", idx, free_list[idx]->size, free_list[idx], free_list[idx]->ptr_to_start);
   }
   printf("\n=== Free List Ends! ===\n");
}

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

// void heap_print(heap_t *heap) {
//     for (int ix = 0; ix < heap_size(heap); ix++) {
//         printf("%3d - %3d : " HEAP_KEY_FORMAT "\n", heap_level(ix), ix,
//                heap->data[ix].key);
//     }
//     printf("\n");
// }

void heap_swap(int index1, int index2)
{
   MemoryBlock *tmp = free_list[index1];
   free_list[index1] = free_list[index2];
   free_list[index2] = tmp;
}

void heap_bubble_up(int index)
{
   int curVal = free_list[index]->size;
   unsigned int parentIdx = heap_parent(index);
   int parentVal = free_list[parentIdx]->size;

   while (index > 0 && parentVal > curVal)
   {
      heap_swap(parentIdx, index);
      index = parentIdx;
      curVal = free_list[index]->size;
      parentIdx = heap_parent(index);
      parentVal = free_list[parentIdx]->size;
   }
}

void heap_bubble_down(int index)
{
   while (index < heap_size)
   {
      unsigned int leftChildIdx = heap_left_child(index);
      unsigned int rightChildIdx = heap_right_child(index);

      unsigned long long curVal = free_list[index]->size;
      unsigned long long leftChildVal = (leftChildIdx < heap_size) ? free_list[leftChildIdx]->size : ULLONG_MAX;
      unsigned long long rightChildVal = (rightChildIdx < heap_size) ? free_list[rightChildIdx]->size : ULLONG_MAX;

      if (curVal <= leftChildVal && curVal <= rightChildVal)
      {
         break;
      }

      if (leftChildVal < rightChildVal)
      {
         heap_swap(index, leftChildIdx);
         index = leftChildIdx;
      }
      else
      {
         heap_swap(index, rightChildIdx);
         index = rightChildIdx;
      }
   }
}

void heap_insert(MemoryBlock *ptr)
{
   if (heap_size >= MAX_SIZE)
   {
      fprintf(stderr, "Error: Free list is full!\n");
      return;
   }

   free_list[heap_size] = ptr;

   // heap->data[heap_size(heap)].key = key;
   // heap->data[heap_size(heap)].value = data;
   // heap->size++;

   heap_bubble_up(heap_size);
   heap_size++;
}

MemoryBlock *heap_remove(int idx)
{
   if (heap_size == 0)
   {
      return NULL;
   }

   MemoryBlock *block = free_list[idx];
   heap_size--;
   free_list[idx] = free_list[heap_size];

   // heap_value_t min = heap->data[0].value;

   // heap->size--;

   // move last element to the root
   // heap->data[0] = heap->data[heap_size(heap)];

   // then bubble it down to its correct position
   heap_bubble_down(idx);

   return block;
}