#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #include "customMalloc.h"

int strlen2(char* str) {
   if (*str == '\0') {
      printf("End of string\n");
      return 0;
   } else {
      int next_len = strlen2(&str[1]);
      printf(
          "Character: %c, remaining string: %s, length of remaining string: "
          "%d, total length: %d\n",
          *str, &str[1], next_len, next_len + 1);
      return 1 + next_len;
   }
}

int main(int argc, char* argv[]) {
   char str[] = "abc";

   char* ptr;
   ptr = (char*)malloc(3 * strlen(str) + 1);
   strcpy(ptr, str);
   strcat(ptr, str);
   strcat(ptr, str);
   printf("Length of string: %d\n", strlen2(ptr));

   free(ptr);

   // ptr = (char*)realloc(ptr, 4 * strlen(str) + 1);
   // strcat(ptr, str);
   // printf("Length of string: %d\n", strlen2(ptr));

   // free(ptr);

   // ptr = (char*)calloc(10, sizeof(int));
   // printf("Length of string: %d\n", strlen2(ptr));
}