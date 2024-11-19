#include "customMalloc.h"

// keeps track of the free blocks
MemoryBlock free_list[MAX_SIZE];
int current_index = 0;

void* malloc(size_t size) {
   if (size == 0) {
      return NULL;
   }

   MemoryBlock* memory_block_ptr = find_block_from_free_list(free_list, size);
   if (memory_block_ptr == NULL) {
      memory_block_ptr = get_me_blocks(size);
   }

   memory_block_ptr = break_block(free_list, memory_block_ptr, size);

   if (memory_block_ptr == NULL) {
      fprintf(stderr, "Error: Memory block is NULL!\n");
      return NULL;
   }

   fprintf(stderr, "Memory block size: %ld\n", memory_block_ptr->size);

   return (void*)(memory_block_ptr);
}

void* realloc(void* ptr, size_t size);
void* calloc(size_t num, size_t size);

MemoryBlock* find_block_from_free_list(MemoryBlock* free_list_arr_ptr,
                                       size_t size) {
   for (int idx = 0; idx < current_index; idx++) {
      if (free_list_arr_ptr[idx].size >= size) {
         MemoryBlock* removed_block_ptr =
             remove_block_from_free_list(free_list_arr_ptr, idx);
         return removed_block_ptr;
      }
   }
   return NULL;
}

MemoryBlock* get_me_blocks(__ssize_t how_much) {
   __ssize_t required_size = (how_much + ONE_GB - 1) & ~(ONE_GB - 1);

   void* ptr = sbrk(required_size);
   if (ptr == (void*)-1) {
      perror("sbrk failed");
      return NULL;
   }

   MemoryBlock* memory_block_ptr = (MemoryBlock*)ptr;
   memory_block_ptr->size = required_size;

   return memory_block_ptr;
}

MemoryBlock* remove_block_from_free_list(MemoryBlock* free_list_arr_ptr,
                                         int idx) {
   if (idx < 0 || idx >= current_index) {
      fprintf(stderr, "Index out of bounds.\n");
      return NULL;
   }

   MemoryBlock* removed_memory_block_ptr = &free_list_arr_ptr[idx];

   for (int i = idx; i < current_index - 1; i++) {
      free_list_arr_ptr[i] = free_list_arr_ptr[i + 1];
   }

   current_index--;

   return removed_memory_block_ptr;
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

void free(void* ptr) {
   if (ptr == NULL) {
      return;
   }

   MemoryBlock* memory_block_ptr = (MemoryBlock*)ptr;

   add_block_to_free_list(free_list, ptr);
}

void print_free_list(void* free_list) {
   MemoryBlock* free_list_arr_ptr = (MemoryBlock*)free_list;
   for (int idx = 0; idx < current_index; idx++) {
      printf("Block %d: %ld\n", idx, free_list_arr_ptr[idx].size);
   }
}