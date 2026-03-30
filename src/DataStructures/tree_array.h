#include <stdio.h>
#include <stdlib.h>
#include "node.h"

struct Tree_array{
    struct Node *node;
    size_t count;
    size_t capacity;
};

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int arr[], int low, int high) {
    int pivot = arr[low];
    int i = low + 1;
    int j = high;

    while (1) {
        while (i <= high && arr[i] <= pivot) i++;
        while (arr[j] > pivot) j--;
        if (i >= j) break;
        swap(&arr[i], &arr[j]);
    }
    swap(&arr[low], &arr[j]);
    return j;
}

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int p = partition(arr, low, high);
        quickSort(arr, low, p - 1); 
        quickSort(arr, p + 1, high);
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
        printf("Data: %d    Frequency: %d\n", tree_array.node[i].pair.data, tree_array.node[i].pair.frequency);
    }
}