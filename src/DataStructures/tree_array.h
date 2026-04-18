#include <stdio.h>
#include <stdlib.h>
#include "node.h"

struct Tree_array{
    size_t count;
    size_t capacity;
    struct Node *node;
};

void swapNode(struct Node *node1, struct Node *node2){
    struct Node temp;
    temp = *node1;
    *node1 = *node2;
    *node2 = temp;
}

void merge(struct Tree_array *tree_array){
    struct Node *node1 = malloc(sizeof(struct Node));
    struct Node *node2 = malloc(sizeof(struct Node));
    struct Node merged;

    *node1 = tree_array->node[tree_array->count - 2];
    *node2 = tree_array->node[tree_array->count - 1];
    merged.pair.data = 0;
    merged.pair.frequency = node1->pair.frequency + node2->pair.frequency;
    merged.left = node1;
    merged.right = node2;

    tree_array->count--;
    tree_array->node[tree_array->count-1] = merged;
}

void addNodeInTreeArray(struct Tree_array *tree_array, struct Pair pairData){
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
    
   tree_array->node[tree_array->count].left = NULL;
   tree_array->node[tree_array->count].right = NULL;
   tree_array->node[tree_array->count++].pair = pairData;
}

void tempPrint(struct Tree_array tree_array){
    for(int i = 0; i < tree_array.count; i++){
        printf("Data: %c    Frequency: %d\n", tree_array.node[i].pair.data, tree_array.node[i].pair.frequency);
    }
}

void reverse(struct Tree_array *tree_array){
    int i = 0;
    int j = tree_array->count - 1;

    while(i < j){
        swapNode(&tree_array->node[i], &tree_array->node[j]);
        i++;
        j--;
    }
}

int partition(struct Tree_array *tree_array, int low, int high) {
    int pivot = tree_array->node[low].pair.frequency;
    int i = low - 1;
    int j = high + 1;

    while (1) {
        do {
            i++;
        } while (tree_array->node[i].pair.frequency < pivot);

        do {
            j--;
        } while (tree_array->node[j].pair.frequency > pivot);

        if (i >= j)
            return j;

        swapNode(&tree_array->node[i], &tree_array->node[j]);
    }
}

void quickSort(struct Tree_array *tree_array, int low, int high) {
    if (low < high) {
        int p = partition(tree_array, low, high);
        quickSort(tree_array, low, p - 1); 
        quickSort(tree_array, p + 1, high);
    }
}

void printTree(struct Node root){

    if(root.pair.data == 0){
        printf("%d ", root.pair.data);
    }
    else{
        printf("%c ", root.pair.data);
    }
    if(root.left){
        printTree(*root.left);
    }
    if(root.right){
        printTree(*root.right);
    }
}

void buildTree(struct Tree_array *tree_array){
    while(tree_array->count >= 2){
        quickSort(tree_array, 0, tree_array->count - 1);
        reverse(tree_array);
        merge(tree_array);
    }
    struct Node *newNode = realloc(tree_array->node, sizeof(struct Node));
    if(newNode == NULL){
        printf("Failed to reallocate memory!");
    }
    else{
        tree_array->node = newNode;
    }
    printf("Printing Tree:\n");
    printTree(tree_array->node[0]);
    printf("\n\nTree printed\n");
}