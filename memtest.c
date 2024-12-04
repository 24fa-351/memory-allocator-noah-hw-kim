#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// define "SYSTEM_MALLOC" for system malloc, include for our versions. Don't do both.
#ifdef SYSTEM_MALLOC
// Use standard library functions
#else
#include "malloc.h"
#define malloc xmalloc
#define realloc xrealloc
#define free xfree
#endif

#define TEST_SIZE 5
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MIN_LARGE_SIZE 1024
#define MAX_LARGE_SIZE 100 * 1024 * 1024

int rand_between(int min, int max) { return rand() % (max - min + 1) + min; }

int main(int argc, char *argv[])
{
   srand(time(NULL));

   char *test_string =
       "Now is the time for all good people to come to the aid "
       "of their country.";

   if (argc > 1)
   {
      test_string = argv[1];
   }

   char *ptrs[TEST_SIZE];

   for (int ix = 0; ix < TEST_SIZE; ix++)
   {
      int size = rand_between(1, strlen(test_string) + 1);

      // sometimes, allocate a large block
      int do_occasionally_large = rand_between(0, 1);

      if (do_occasionally_large)
      {
         size = rand_between(MIN_LARGE_SIZE, 100 * 1024 * 1024);
      }

      fprintf(stderr, "\n\n\n[%d] size: %d\n", ix, size);

      ptrs[ix] = malloc(size);
      if (ptrs[ix] == NULL)
      {
         printf("[%d] malloc failed\n", ix);
         exit(1);
      }

      int len_to_copy = MIN(strlen(test_string), size - 1);

      fprintf(stderr, "[%d] ptrs[%d]: %p, going to copy %d chars\n", ix, ix,
              ptrs[ix], len_to_copy);

      strncpy(ptrs[ix], test_string, len_to_copy);
      ptrs[ix][len_to_copy] = '\0';

      fprintf(stderr, "[%x] '%s'\n", ix, ptrs[ix]);

      int index_to_free = rand_between(0, ix);

      // sometimes reallocate
      int do_realloc = rand_between(0, 1);

      if (do_realloc)
      {
         int new_size = rand_between(1, strlen(test_string) + 1);
         fprintf(stderr, "[%d] reallocating %p to size %d\n", ix, ptrs[ix],
                 new_size);
         ptrs[ix] = realloc(ptrs[ix], new_size);
         if (ptrs[ix] == NULL)
         {
            printf("[%d] realloc failed\n", ix);
            exit(1);
         }
      }

      // fprintf(stderr, "[%x] '%s'\n", ix, ptrs[ix]);
      if (ptrs[index_to_free])
      {
         // fprintf(stderr, "[%x] '%s'\n", ix, ptrs[ix]);
         fprintf(stderr, "\n[%d] randomly freeing %p ('%s')\n", index_to_free,
                 ptrs[index_to_free], ptrs[index_to_free]);
         free(ptrs[index_to_free]);
         fprintf(stderr, "[%d] freed %p\n", index_to_free, ptrs[index_to_free]);
         ptrs[index_to_free] = NULL;
      }
   }

   for (int ix = 0; ix < TEST_SIZE; ix++)
   {
      if (ptrs[ix])
      {
         fprintf(stderr, "[%d] freeing %p (%s)\n", ix, ptrs[ix], ptrs[ix]);
         free(ptrs[ix]);
         fprintf(stderr, "[%d] freed %p\n", ix, ptrs[ix]);
      }
      else
      {
         fprintf(stderr, "[%d] already freed\n", ix);
      }
   }

   return 0;
}
