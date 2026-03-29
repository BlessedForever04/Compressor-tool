#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "pair.h"

struct Frequency_array{
   struct Pair *item;
   size_t count;
   size_t capacity; 
};

void add(struct Frequency_array *array, uint8_t data){

   for(size_t i = 0; i < array->count; i++){
      if(array->item[i].data == data){
         array->item[i].frequency++;
         return;
      }
   }

   if(array->count == array->capacity){
      if(array->capacity == 0){
         array->capacity = 1;
      } else {
         array->capacity *= 2;
      }

      struct Pair *newItem = realloc(array->item,
                                     array->capacity * sizeof(struct Pair));
      if(newItem == NULL){
         printf("Memory allocation failed\n");
         exit(1);
      }

      array->item = newItem;
   }

   array->item[array->count].data = data;
   array->item[array->count].frequency = 1;
   array->count++;
}