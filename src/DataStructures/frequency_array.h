#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "pair.h"

struct Frequency_array{
   struct Pair *pair;
   size_t count;
   size_t capacity; 
};

void addPair(struct Frequency_array *array, uint8_t data){

   for(size_t i = 0; i < array->count; i++){
      if(array->pair[i].data == data){
         array->pair[i].frequency++;
         return;
      }
   }

   if(array->count == array->capacity){
      if(array->capacity == 0){
         array->capacity = 1;
      } else {
         array->capacity *= 2;
      }

      struct Pair *newpair = realloc(array->pair, array->capacity * sizeof(struct Pair));
      if(newpair == NULL){
         printf("Memory allocation failed\n");
         exit(1);
      }

      array->pair = newpair;
   }

   array->pair[array->count].data = data;
   array->pair[array->count].frequency = 1;
   array->count++;
}

void customPrint(struct Frequency_array array){
    int temp = 0;
    while(temp != array.count){
        printf("Data: %d Frequency: %d\n", array.pair[temp].data, array.pair[temp].frequency);
        temp++;
    }
}