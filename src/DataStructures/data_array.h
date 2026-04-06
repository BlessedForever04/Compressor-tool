#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

struct DataArray{
    int *data;
    size_t count;
    size_t capacity;
};

void addData(struct DataArray *dataArray, int data){
    if(dataArray->count == dataArray->capacity){
        if(dataArray->capacity == 0){
            dataArray->capacity = 1;
        }
        else{
            dataArray->capacity = dataArray->capacity*2;
        }

        int *newData = realloc(dataArray->data, dataArray->capacity * (sizeof(int)));
        if(newData == NULL){
            printf("Failed to allocate memory!");
            return;
        }
        dataArray->data = newData;
    }

    dataArray->data[dataArray->count++] = data;
}