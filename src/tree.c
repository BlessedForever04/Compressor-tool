#include <stdint.h>

struct Node{
    int frequency;
    uint8_t byte;
    struct Node *left;
    struct Node *right;
};