#include "malloc.h"

// keeps track of the free blocks
MemoryBlock free_list[MAX_SIZE];
int current_index = 0;

void* xmalloc(size_t size) {
   if (size == 0) {
      return NULL;
   }

   fprintf(stderr, "Requested size from user: %ld\n", size);

   MemoryBlock* memory_block_ptr = find_block_from_free_list(free_list, size);
   if (memory_block_ptr == NULL) {
      fprintf(stderr, "Memory block not found in free list.\n");
      memory_block_ptr = get_me_blocks(size);
      // fprintf(stderr, "We got memory block size from system: %ld\n",
      //         memory_block_ptr->size);
   }
   // else {
   //    fprintf(stderr, "Memory block found in free list.\n");
   //    fprintf(stderr, "Found memory block size: %ld\n",
   //    memory_block_ptr->size);
   // }

   memory_block_ptr = break_block(free_list, memory_block_ptr, size);

   if (memory_block_ptr == NULL) {
      fprintf(stderr, "Error: Memory block is NULL!\n");
      return NULL;
   }

   print_free_list(free_list);

   return (void*)(memory_block_ptr);
}

void* realloc(void* ptr, size_t size);
void* calloc(size_t num, size_t size);

MemoryBlock* find_block_from_free_list(MemoryBlock* free_list_arr_ptr,
                                       size_t size) {
   for (int idx = 0; idx < current_index; idx++) {
      if (free_list_arr_ptr[idx].size >= size) {
         fprintf(stderr, "Found block at index: %d\n", idx);
         fprintf(stderr, "Block size: %ld\n", free_list_arr_ptr[idx].size);
         MemoryBlock removed_block =
             remove_block_from_free_list(free_list_arr_ptr, idx);

         if (removed_block.size == 0) {
            fprintf(stderr, "Error: Removed block size is 0!\n");
            return NULL;
         }

         // fprintf(stderr, "Removed block size: %ld\n",
         // removed_block_ptr->size);
         return &removed_block;
      }
   }
   return NULL;
}

MemoryBlock* get_me_blocks(size_t how_much) {
   size_t required_size = how_much + PADDING_SIZE;

   if (required_size < MIN_REQUEST_SIZE) {
      required_size = MIN_REQUEST_SIZE;
   }

   void* ptr = sbrk(required_size + PADDING_SIZE);
   if (ptr == (void*)-1) {
      perror("sbrk failed");
      return NULL;
   }

   MemoryBlock* memory_block_ptr = (MemoryBlock*)ptr;
   memory_block_ptr->size = required_size;

   return memory_block_ptr;
}

MemoryBlock remove_block_from_free_list(MemoryBlock* free_list_arr_ptr,
                                        int idx) {
   if (idx < 0 || idx >= current_index) {
      fprintf(stderr, "Index out of bounds.\n");
      return (MemoryBlock){0};
   }

   MemoryBlock removed_memory_block = free_list_arr_ptr[idx];
   fprintf(stderr, "Removed block size before overwrite array: %ld\n",
           removed_memory_block.size);

   for (int i = idx; i < current_index - 1; i++) {
      free_list_arr_ptr[i] = free_list_arr_ptr[i + 1];
   }

   current_index--;

   fprintf(stderr, "Removed block size after overwrite array: %ld\n",
           removed_memory_block.size);

   return removed_memory_block;
}

MemoryBlock* break_block(MemoryBlock* free_list_arr_ptr,
                         MemoryBlock* memory_block_ptr, size_t size) {
   if (memory_block_ptr->size < size) {
      fprintf(stderr,
              "Block size is less than required size.\nRequested size: %ld, "
              "Block size: %ld\n",
              size, memory_block_ptr->size);
      return NULL;
   } else if (memory_block_ptr->size == size) {
      return memory_block_ptr;
   } else {
      MemoryBlock* new_memory_block_ptr =
          (MemoryBlock*)((char*)memory_block_ptr + size);
      new_memory_block_ptr->size = memory_block_ptr->size - size;

      add_block_to_free_list(free_list_arr_ptr, new_memory_block_ptr);

      memory_block_ptr->size = size;

      current_index++;

      return memory_block_ptr;
   }
}

void add_block_to_free_list(MemoryBlock* free_list, MemoryBlock* ptr) {
   if (current_index < MAX_SIZE) {
      free_list[current_index] = *ptr;
      current_index++;
   } else {
      fprintf(stderr, "Error: Free list is full!\n");
   }
}

void xfree(void* ptr) {
   if (ptr == NULL) {
      return;
   }

   MemoryBlock* memory_block_ptr = (MemoryBlock*)ptr;

   add_block_to_free_list(free_list, ptr);

   print_free_list(free_list);
}

void print_free_list(void* free_list) {
   MemoryBlock* free_list_arr_ptr = (MemoryBlock*)free_list;
   for (int idx = 0; idx < current_index; idx++) {
      printf("Block %d: %ld\n", idx, free_list_arr_ptr[idx].size);
   }
}