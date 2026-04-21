#include <stdlib.h>

typedef struct My_string{
    unsigned char *item;
    size_t count;
    size_t capacity;
}My_string_t;

void appendChar(My_string_t *buffer, unsigned char character){
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

void appendStr(My_string_t *mainStr, My_string_t Str){
    for(int i = 0; i < Str.count; i++){
        appendChar(mainStr, Str.item[i]);
    }
}

void appendNormalStr(My_string_t *mainStr, char *normalStr, size_t normalStrSize){
    for(int i = 0; i < normalStrSize; i++){
        appendChar(mainStr, normalStr[i]);
    }
}

void format(My_string_t *temp){
    temp->count = 0;
}