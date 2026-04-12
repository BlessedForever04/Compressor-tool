#include <stdlib.h>

struct My_string{
    char *item;
    size_t count;
    size_t capacity;
};

void appendChar(struct My_string *buffer, char character){
    if(buffer->count >= buffer->capacity){
        if(buffer->capacity == 0){
            buffer->capacity = 1;
        }
        else{
            buffer->capacity *= 2;
        }
        
        char *newItem = realloc(buffer->item, buffer->capacity * sizeof(char));
        buffer->item = newItem;
    }
    
    buffer->item[buffer->count++] = character;
}

void appendStr(struct My_string *buffer, struct My_string temp){
    for(int i = 0; i < temp.count; i++){
        appendChar(buffer, temp.item[i]);
    }
}

void format(struct My_string *temp){
    temp->count = 0;
}