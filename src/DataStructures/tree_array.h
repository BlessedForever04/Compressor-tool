#include <stdio.h>
#include <stdlib.h>
#include "node.h"

typedef struct Tree_array{
    size_t count;
    size_t capacity;
    Node_t *node;
}Tree_array_t;

void swapNode(Node_t*node1, Node_t *node2){
    Node_t temp;
    temp = *node1;
    *node1 = *node2;
    *node2 = temp;
}

void merge(Tree_array_t *tree_array){
    Node_t *node1 = malloc(sizeof(Node_t));
    Node_t *node2 = malloc(sizeof(Node_t));
    Node_t merged;

    *node1 = tree_array->node[tree_array->count - 2];
    *node2 = tree_array->node[tree_array->count - 1];
    merged.pair.data = 0;
    merged.pair.frequency = node1->pair.frequency + node2->pair.frequency;
    merged.left = node1;
    merged.right = node2;

    tree_array->count--;
    tree_array->node[tree_array->count-1] = merged;
}

void addNodeInTreeArray(Tree_array_t *tree_array, Pair_t pairData){
   if(tree_array->count == tree_array->capacity){
      if(tree_array->capacity == 0){
         tree_array->capacity = 1;
      } else {
         tree_array->capacity *= 2;
      }

      Node_t *newNode = realloc(tree_array->node, tree_array->capacity * sizeof(Node_t));
      if(newNode == NULL){
         exit(1);
      }
      tree_array->node = newNode;
   }
    
   tree_array->node[tree_array->count].left = NULL;
   tree_array->node[tree_array->count].right = NULL;
   tree_array->node[tree_array->count++].pair = pairData;
}

void tempPrint(Tree_array_t tree_array){
    for(int i = 0; i < tree_array.count; i++){
        printf("Data: %c    Frequency: %d\n", tree_array.node[i].pair.data, tree_array.node[i].pair.frequency);
    }
}

void reverse(Tree_array_t *tree_array){
    int i = 0;
    int j = tree_array->count - 1;

    while(i < j){
        swapNode(&tree_array->node[i], &tree_array->node[j]);
        i++;
        j--;
    }
}

int partition(Tree_array_t *tree_array, int low, int high) {
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

void quickSort(Tree_array_t *tree_array, int low, int high) {
    if (low < high) {
        int p = partition(tree_array, low, high);
        quickSort(tree_array, low, p - 1); 
        quickSort(tree_array, p + 1, high);
    }
}

void printTree(Node_t root){

    if(root.pair.data == 0){
        printf("%d ", root.pair.data);
    }
    else{
        printf("%d ", root.pair.data);
    }
    if(root.left){
        printTree(*root.left);
    }
    if(root.right){
        printTree(*root.right);
    }
}

void buildTree(Tree_array_t *tree_array){
    while(tree_array->count >= 2){
        quickSort(tree_array, 0, tree_array->count - 1);
        reverse(tree_array);
        merge(tree_array);
    }
    Node_t *newNode = realloc(tree_array->node, sizeof(Node_t));
    if(newNode == NULL){
        exit(1);
    }
    else{
        tree_array->node = newNode;
    }
}