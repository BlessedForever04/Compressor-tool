#include <stdio.h>
#include <stdint.h>
#include "pair.h"

struct Node{
    struct Node *left;
    struct Node *right;
    struct Pair pair;
};