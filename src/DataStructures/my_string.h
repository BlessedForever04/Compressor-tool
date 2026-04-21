#include <stdlib.h>

struct My_string{
    unsigned char *item;
    size_t count;
    size_t capacity;
};

void appendChar(struct My_string *buffer, unsigned char character){
    if(buffer->count >= buffer->capacity){
        if(buffer->capacity == 0){
            buffer->capacity = 1;
        }
        else{
            buffer->capacity *= 2;
        }
        
        unsigned char *newItem = realloc(buffer->item, buffer->capacity * sizeof(char));
        buffer->item = newItem;
    }
    
    buffer->item[buffer->count++] = character;
}

void appendStr(struct My_string *mainStr, struct My_string Str){
    for(int i = 0; i < Str.count; i++){
        appendChar(mainStr, Str.item[i]);
    }
}

void format(struct My_string *temp){
    temp->count = 0;
}