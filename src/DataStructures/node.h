#include <stdio.h>
#include <stdint.h>
#include "pair.h"

// Huffman tree's node
typedef struct Node{
    struct Node *left;
    struct Node *right;
    Pair_t pair;
}Node_t;