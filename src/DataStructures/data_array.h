#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

struct Data_array{
    uint8_t *data;
    size_t count;
    size_t capacity;
};

void addDataInDataArray(struct Data_array *data_array, uint8_t data){
    if(data_array->count == data_array->capacity){
        if(data_array->capacity == 0){
            data_array->capacity = 1;
        }
        else{
            data_array->capacity = data_array->capacity*2;
        }

        uint8_t *newData = realloc(data_array->data, data_array->capacity * sizeof(uint8_t));
        if(newData == NULL){
            return;
        }
        data_array->data = newData;
    }

    data_array->data[data_array->count++] = data;
}