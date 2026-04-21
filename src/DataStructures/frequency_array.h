#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "pair.h"

typedef struct Frequency_array{
   size_t count;
   size_t capacity; 
   Pair_t *pair;
}Frequency_array_t;

void addPairInFrequencyArray(Frequency_array_t *array, uint8_t data){

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

      Pair_t *newpair = realloc(array->pair, array->capacity * sizeof(Pair_t));
      if(newpair == NULL){
         exit(1);
      }

      array->pair = newpair;
   }

   array->pair[array->count].data = data;
   array->pair[array->count].frequency = 1;
   array->count++;
}

void printFrequencyArray(Frequency_array_t array){
    int temp = 0;
    while(temp != array.count){
        printf("Data: %d Frequency: %d\n", array.pair[temp].data, array.pair[temp].frequency);
        temp++;
    }
}