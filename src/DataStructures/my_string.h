#include <stdlib.h>

// Custom string implementation because why not
typedef struct My_string{
    unsigned char *item;
    size_t count;
    size_t capacity;
}My_string_t;

// appending character in custom string
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

// Appending custom string to custom string
void appendStr(My_string_t *mainStr, My_string_t Str){
    for(int i = 0; i < Str.count; i++){
        appendChar(mainStr, Str.item[i]);
    }
}

// Appending normal string to custom string
void appendNormalStr(My_string_t *mainStr, char *normalStr, size_t normalStrSize){
    for(int i = 0; i < normalStrSize; i++){
        appendChar(mainStr, normalStr[i]);
    }
}

// Format function is basically string.clear() which clears strings
void format(My_string_t *temp){
    temp->count = 0;
}