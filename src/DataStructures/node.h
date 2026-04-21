#include <stdio.h>
#include <stdint.h>
#include "pair.h"

typedef struct Node{
    Node_t *left;
    Node_t *right;
    Pair_t pair;
}Node_t;