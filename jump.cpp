#include <iostream>

void solution(int *array, int size){
    std::cout<<sizeof(array)<<"\n";
    array[20] = 0x202020;
}

int main(){
    // 1 5 4 9 9 3 0 0 1 3 - - 3 1 1 n-1
    int array[] = {1, 5, 4, 9, 9, 3, 0, 0, 1, 3, 0, 0, 3, 1, 1};
    std::cout<<sizeof(array)<<"\n";
    solution(array, 16);
    // std::cout<<"\n"<<sizeof(array);
}

void giveAccess(){
    std::cout<<"I am here";
}