#include "malloc.h"

// reference: https://danluu.com/malloc-tutorial/

// keeps track of the free blocks
MemoryBlock *free_list[MAX_SIZE];
int cur_idx = 0;

void *xmalloc(size_t size)
{
   if (size == 0)
   {
      return NULL;
   }

   // fprintf(stderr, "Requested size from user: %ld\n", size);

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
   for (int idx = 0; idx < cur_idx; idx++)
   {
      if (free_list[idx]->size >= size)
      {
         fprintf(stderr, "Found block at index: %d, size: %ld, address: %p, address_start: %p\n", idx, free_list[idx]->size, free_list[idx], free_list[idx]->ptr_to_start);
         // fprintf(stderr, "Block size: %ld\n", free_list[idx].size);
         MemoryBlock *removed_block_ptr = remove_block_from_free_list(idx);
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

MemoryBlock *remove_block_from_free_list(int idx)
{
   if (idx < 0 || idx >= cur_idx)
   {
      fprintf(stderr, "Index out of bounds.\n");
      return NULL;
   }

   MemoryBlock *removed_memory_block = free_list[idx];
   // fprintf(stderr, "Removed block size before overwrite array: %ld\n",
   //         removed_memory_block.size);

   for (int i = idx; i < cur_idx - 1; i++)
   {
      free_list[i] = free_list[i + 1];
   }

   cur_idx--;

   // fprintf(stderr, "Removed block size after overwrite array: %ld\n",
   //         removed_memory_block.size);

   return removed_memory_block;
}

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

   add_block_to_free_list(extra_memory_block_ptr);

   return memory_block_ptr;
}

void add_block_to_free_list(MemoryBlock *ptr)
{
   if (cur_idx >= MAX_SIZE)
   {
      fprintf(stderr, "Error: Free list is full!\n");
      return;
   }
   // printf("Adding block to free list: %p\n", (void *)ptr);
   free_list[cur_idx] = ptr;
   // printf("Added block to free list: %p\n", (void *)free_list[cur_idx]);
   cur_idx++;
}

void xfree(void *ptr)
{
   if (ptr == NULL)
   {
      return;
   }

   MemoryBlock *memory_block_ptr = (MemoryBlock *)((char *)ptr - sizeof(MemoryBlock));

   // printf("Freeing memory block address: %p\n", memory_block_ptr);

   add_block_to_free_list(memory_block_ptr);

   print_free_list(free_list);
}

void print_free_list()
{
   printf("\n=== Free List Starts! ===\n");
   for (int idx = 0; idx < cur_idx; idx++)
   {
      printf("Block %d: Size - %ld, Address:%p, Address_start: %p\n", idx, free_list[idx]->size, free_list[idx], free_list[idx]->ptr_to_start);
   }
   printf("\n=== Free List Ends! ===\n");
}