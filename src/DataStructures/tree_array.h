#include <stdio.h>
#include <stdlib.h>
#include "node.h"

struct Tree_array{
    struct Node *node;
    size_t count;
    size_t capacity;
};

void swapFrequency(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void swapData(uint8_t *a, uint8_t *b) {
    uint8_t temp = *a;
    *a = *b;
    *b = temp;
}

int partition(struct Tree_array *tree_array, int low, int high) {
    int pivot = tree_array->node[low].pair.frequency;
    int i = low + 1;
    int j = high;

    while (1) {
        while (i <= high && tree_array->node[i].pair.frequency <= pivot) i++;
        while (tree_array->node[j].pair.frequency > pivot) j--;
        if (i >= j) break;
        swapFrequency(&tree_array->node[i].pair.frequency, &tree_array->node[j].pair.frequency);
        swapData(&tree_array->node[i].pair.data, &tree_array->node[j].pair.data);
    }
    swapFrequency(&tree_array->node[low].pair.frequency, &tree_array->node[j].pair.frequency);
    swapData(&tree_array->node[low].pair.data, &tree_array->node[j].pair.data);
    return j;
}

void quickSort(struct Tree_array *tree_array, int low, int high) {
    if (low < high) {
        int p = partition(tree_array, low, high);
        quickSort(tree_array, low, p - 1); 
        quickSort(tree_array, p + 1, high);
    }
}

void merge(){

}

void addNode(struct Tree_array *tree_array, struct Pair pairData){
   if(tree_array->count == tree_array->capacity){
      if(tree_array->capacity == 0){
         tree_array->capacity = 1;
      } else {
         tree_array->capacity *= 2;
      }

      struct Node *newNode = realloc(tree_array->node, tree_array->capacity * sizeof(struct Node));
      if(newNode == NULL){
         printf("Memory allocation failed\n");
         exit(1);
      }
      tree_array->node = newNode;
   }
    
   tree_array->node[tree_array->count++].pair = pairData;
}

void tempPrint(struct Tree_array tree_array){
    for(int i = 0; i < tree_array.count; i++){
        printf("Data: %c    Frequency: %d\n", tree_array.node[i].pair.data, tree_array.node[i].pair.frequency);
    }
}
void buildTree(struct Tree_array *tree_array){
    printf("Before:\n");
    tempPrint(*tree_array);
    quickSort(tree_array, 0, tree_array->count);
    printf("\nAfter:\n");
    tempPrint(*tree_array);
}
