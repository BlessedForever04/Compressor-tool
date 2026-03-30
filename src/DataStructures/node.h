#include <stdio.h>
#include <stdint.h>
#include "pair.h"

struct Node{
    struct Pair pair;
    struct Node *left;
    struct Node *right;
};