#include <stdio.h>
#include <stdlib.h>
#include "DataStructures\frequency_array.h"
#include "DataStructures\tree_array.h"
#include "DataStructures\data_array.h"
#include "DataStructures\my_string.h"
// <flag bit of h/n type> <tree start flag> <tree> <tree end flag> <frequency 
struct Frequency_array frequency_array;
struct Tree_array tree_array = {0, 0, NULL};
struct Data_array data_array = {NULL, 0, 0};

void decompress(){
    //find bit flag (h/n type)
    // convert byptes into bits -> iterate till legal count -> traverse tree using the path -> get real bytes -> output 
}

void highCompression(){
    //LZMA
}
// lets create a array storing a pair of byte and its frequency
// Then I'll sort it
// then I have to create tree based on that frequency
void calculateFrequencies(){
    int character;
    while((character = getchar()) != EOF){
        addDataInDataArray(&data_array, character);
        addPairInFrequencyArray(&frequency_array, character); 
    }
}

void createHuffmanTree(){
    for(int i = 0; i < frequency_array.count; i++){
        addNodeInTreeArray(&tree_array, frequency_array.pair[i]);
    }
    buildTree(&tree_array);  
}

char createEncoding(struct Node root, char target, struct My_string *temp){
    if(root.left == NULL && root.right == NULL){
        if(root.pair.data == target){
            printf("FOUND %c\n", root.pair.data);
            return '1';
        }
        else{
            printf("Target miss so revert T:%c N:%c\n", target, root.pair.data);
            temp->count--;
        }
    }
    else{
        appendChar(temp, '0');
        printf("Went left\n");
        if(createEncoding(*root.left, target, temp) == '1'){
            return '1';
        }
        printf("Went right\n");
        appendChar(temp, '1');
        if(createEncoding(*root.right, target, temp) == '1'){
            return '1';
        }
        printf("NOT LR BACK\n");
        temp->count--;
    }
}

void createDataBuffer(){
    // Binary encoding of actual data using tree
    struct My_string temp = {NULL, 0, 0};
    struct My_string encoding = {NULL, 0, 0};

    for(int i = 0; i < data_array.count; i++){
        createEncoding(tree_array.node[0], data_array.data[i], &temp);
        appendStr(&encoding, temp);
        format(&temp);
    }// At this point, i have proper encoding created out of tree (0/1) XD
}

void storeTree(struct Node *root){
    if(!root) return;

    if(root->left == NULL && root->right == NULL){
        putchar(1);
        putchar(root->pair.data);
    }
    else{
        putchar(0);
        storeTree(root->left);
        storeTree(root->right);
    } 
}

void output(){
    putchar(0); // Flag of compression type
    putchar((frequency_array.count*2 + frequency_array.count - 1)); // Size of tree buffer
    storeTree(&tree_array.node[0]); // serialization
    createDataBuffer(); 
}

void normalCompression(){
    //huffman
    // Read bytes -> get its frequency
    // -> build huffman tree -> generate code (0 / 1) -> bit packing and legal count -> output
    calculateFrequencies();
    createHuffmanTree();
    output();
}

void help(){
    printf("\nCurrent commands in use :\n\n");
    printf("high file compression       ./compressor -c -h < input_file_name.extension > output_file_name.custom_extension\n");
    printf("Normal file compression     ./compressor -c -n < input_file_name.extension > output_file_name.custom_extension\n");
    printf("Decompress file             ./compressor -d < input_file_name.extension > output_file_name.custom_extension\n");
    printf("About                       ./compressor -about\n");
    printf("GitHub repo                 ./compressor -github\n");
    printf("\nFor more information about tool, checkout github repo.\n\n");
}

void error(){
    printf("Incorrent command, try ./main -help\n");
}

void github(){
    system("start \"\" \"https://github.com/BlessedForever04/Compressor-tool\"");
}

void about(){
    FILE *fptr = popen("curl -s https://raw.githubusercontent.com/BlessedForever04/Compressor-tool/main/README.md?t=1d", "r");
    char response[1024];
    while(fgets(response, sizeof(response), fptr)){
        char *character = response;
        while(*character != '\n'){
            if((*character != '#') && (*character != '`') && (*character != '*')){
                printf("%c", *character);
            }
            character++;
        }
        printf("\n");
    }
}