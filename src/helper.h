#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "DataStructures\frequency_array.h"
#include "DataStructures\tree_array.h"
#include "DataStructures\data_array.h"
#include "DataStructures\my_string.h"

struct Frequency_array frequency_array;
struct Tree_array tree_array = {0, 0, NULL};
struct Data_array data_array = {NULL, 0, 0};

// type flag('0':Huffman/ '1':LZMA)(char) + Tree length (n uint32) + tree data (serialized) + Valid count (uint32) + Encoding


void rebuildTree(uint32_t treeLength, FILE *file){
    uint8_t treeBuffer[treeLength];
    fread(treeBuffer, sizeof(uint8_t), treeLength, file); // no need of pointer as array = &array[0]
    
}

void decompress(struct My_string fileName){
    //find bit flag (h/n type)
    FILE *file = fopen(fileName.item, "rb");
    
    if(file == NULL){
        printf("File was not opened properly");
        exit(1);
    }
    
    uint8_t compressionFlag;
    fread(&compressionFlag, 1, 1, file);

    if(compressionFlag == 0){
        // huffman decompression
        uint32_t treeLength = 0;
        fread(&treeLength, sizeof(uint32_t), 1, file);
        struct Node root = {NULL, NULL, {0, 0}};
        rebuildTree(treeLength, file);
    }
    else{
        if(compressionFlag == 1){

        }
        else{

        }
        // lzma decompression
    }
    fclose(file);
    // convert byptes into bits -> iterate till legal count -> traverse tree using the path -> get real bytes -> output 
}

void highCompression(struct My_string fileName){
    //LZMA
    FILE *outputFile;
    fputc('1', outputFile);
}

void calculateFrequencies(struct My_string fileName){
    FILE *file = fopen(fileName.item, "rb");
    if(file == NULL){
        perror("File not opened correctly!");
        exit(1);
    }

    unsigned char buffer[1024];

    size_t bytesCount = 0;

    while((bytesCount = fread(buffer, sizeof(unsigned char), 1024, file)) > 0){
        for(int i = 0; i < bytesCount; i++){
            addDataInDataArray(&data_array, buffer[i]);
            addPairInFrequencyArray(&frequency_array, buffer[i]);
        }
    }

    fclose(file);
}

void createHuffmanTree(){
    for(int i = 0; i < frequency_array.count; i++){
        addNodeInTreeArray(&tree_array, frequency_array.pair[i]);
    }
    buildTree(&tree_array); 
}

int createEncoding(struct Node root, uint8_t target, struct My_string *temp){
    if(root.left == NULL && root.right == NULL){
        if(root.pair.data == target){
            return 1;
        }
        return 0;
    }

    size_t baseCount = temp->count;

    appendChar(temp, '0');
    if(createEncoding(*root.left, target, temp)){
        return 1;
    }
    temp->count = baseCount;
    appendChar(temp, '1');
    if(createEncoding(*root.right, target, temp)){
        return 1;
    }
    temp->count = baseCount;
    return 0;
}

void bitPacking(struct My_string encoding, FILE *outputFile){
    unsigned char byte = 0;
    uint8_t count = 0;
    
    for(int i = 0; i < encoding.count; i++){
        count++;
        byte = (byte << 1) | (encoding.item[i] - '0');
        if(count == 8){
            count = 0;
            fputc(byte, outputFile);
            byte = 0;
        }
    }
}

void createDataBuffer(FILE *outputFile){
    // Binary encoding of actual data using tree
    uint32_t validBit = 0;
    struct My_string temp = {NULL, 0, 0};
    struct My_string encoding = {NULL, 0, 0};

    for(int i = 0; i < data_array.count; i++){
        if(!createEncoding(tree_array.node[0], data_array.data[i], &temp)){
            fprintf(stderr, "Encoding failed for byte value %u\n", data_array.data[i]);
            exit(1);
        }

        // Single-symbol files need at least one bit per symbol.
        if(temp.count == 0){
            appendChar(&temp, '0');
        }

        appendStr(&encoding, temp);
        format(&temp);
    }// At this point, i have proper encoding created out of tree (0/1) XD

    validBit = encoding.count;
    fwrite(&validBit, sizeof(uint32_t), 1, outputFile);
    // If encoding is not multiple of 8 then bit packing will not be done
    while(encoding.count % 8 != 0){
        appendChar(&encoding, '0');
    }
    // bit packing
    bitPacking(encoding, outputFile);
}

void storeTree(struct Node *root, FILE *outputFile){
    if(!root) return;

    if(root->left == NULL && root->right == NULL){
        fputc((uint8_t)1, outputFile);
        fputc(root->pair.data, outputFile);
    }
    else{
        fputc((uint8_t)0, outputFile);
        storeTree(root->left, outputFile);
        storeTree(root->right, outputFile);
    } 
}

void output(struct My_string fileName){
    struct My_string rawOutputFileName = {NULL, 0, 0};
    for(int i = 0; i < fileName.count; i++){
        if(fileName.item[i] == '.'){
            break;
        }
        else{
            appendChar(&rawOutputFileName, fileName.item[i]);
        }
    }
    struct My_string extension = {".compressed", 11, 12};
    appendStr(&rawOutputFileName, extension);
    appendNormalStr(&rawOutputFileName, "\0", 1);

    printf("Output file name is: %s", rawOutputFileName);

    unsigned char outputFileName[rawOutputFileName.count];

    for(int i = 0; i < rawOutputFileName.count; i++){
        outputFileName[i] = rawOutputFileName.item[i];
    }

    FILE *outputFile = fopen(outputFileName, "wb");

    fputc((uint8_t)0, outputFile);// Flag of huffman compression type
    uint32_t treeLength = (uint32_t)(frequency_array.count*2 + frequency_array.count - 1);
    fwrite(&treeLength, sizeof(uint32_t), 1, outputFile); // Size of tree buffer - just iterate till this count and you get your ccomplete tree 
    storeTree(&tree_array.node[0], outputFile); // serialization
    createDataBuffer(outputFile);
    fclose(outputFile);
}

void huffmanCompression(struct My_string fileName){
    //huffman
    // Read bytes -> get its frequency
    // -> build huffman tree -> generate code (0 / 1) -> bit packing and legal count -> output
    calculateFrequencies(fileName);
    createHuffmanTree();
    output(fileName);
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
    unsigned char response[1024];
    while(fgets(response, sizeof(response), fptr)){
        unsigned char *character = response;
        while(*character != '\n'){
            if((*character != '#') && (*character != '`') && (*character != '*')){
                printf("%c", *character);
            }
            character++;
        }
        printf("\n");
    }
}