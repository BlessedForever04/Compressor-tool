#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "DataStructures\frequency_array.h"
#include "DataStructures\tree_array.h"
#include "DataStructures\data_array.h"
#include "DataStructures\my_string.h"

Frequency_array_t frequency_array;
Tree_array_t tree_array = {0, 0, NULL};
Data_array_t data_array = {NULL, 0, 0};

// name length (size_t) + name.extension (char*) + type flag('0':Huffman/ '1':LZMA)(char) + Tree length (n uint32) + tree data (serialized) + Valid count (uint32) + Encoding

void deserializeTree(Node_t *currentNode, uint8_t treeBuffer[], uint32_t *currentBufferIndex, uint32_t treeLength){
    if(*currentBufferIndex < treeLength){
        uint8_t byte = treeBuffer[(*currentBufferIndex)++];

        if(byte == 0){
            currentNode->pair.data = 0;
            currentNode->pair.frequency = 0;
            Node_t *leftNode = malloc(sizeof(Node_t));
            Node_t *rightNode = malloc(sizeof(Node_t));

            leftNode->left = NULL;
            leftNode->right = NULL;
            rightNode->left = NULL;
            rightNode->right = NULL;

            currentNode->left = leftNode;
            deserializeTree(currentNode->left, treeBuffer, currentBufferIndex, treeLength);
            currentNode->right = rightNode;
            deserializeTree(currentNode->right, treeBuffer, currentBufferIndex, treeLength);
        }
        else{
            if(byte == 1){
                currentNode->pair.data = treeBuffer[(*currentBufferIndex)++];
                currentNode->pair.frequency = 0;
                currentNode->left = NULL;
                currentNode->right = NULL;
            }
            else{
                printf("\nReading wrong stuff: %d\n", byte);
            }
        }
    }
}

void rebuildTree(uint32_t treeLength, FILE *file, Node_t *root){
    uint8_t treeBuffer[treeLength];
    fread(treeBuffer, sizeof(uint8_t), treeLength, file); // no need of pointer as array = &array[0]
    uint32_t currentBufferIndex = 0;
    deserializeTree(root, treeBuffer, &currentBufferIndex, treeLength);
    // printTree(*root);
}

uint8_t* getEncoding(uint32_t encodingSize, FILE *compressedFile){
    uint8_t *encoding = malloc(sizeof(uint8_t)*encodingSize);
    fread(encoding, sizeof(uint8_t), encodingSize, compressedFile);
    return encoding;
}

void decode(uint8_t *encoding, uint32_t encodingSize, My_string_t *actualData){
    for(int i = 0; i < encodingSize; i++){
        uint8_t byte = encoding[i];

        for(int bit = 7; bit >= 0; bit--){
            char c = ((byte>>bit) & 1) ? '1' : '0';
            appendChar(actualData, c);
        }
    }
}

void generateOutput(My_string_t *output, uint32_t validBit, My_string_t actualData, Node_t *root, FILE *outputFile){
    Node_t *currentNode = root;
    for(int i = 0; i <= validBit; i++){
        if(currentNode->left == NULL && currentNode->right == NULL){
            //found
            uint8_t character = currentNode->pair.data;
            fwrite(&character, sizeof(uint8_t), 1, outputFile);
            currentNode = root;
            i--;
        }
        else{
            if(actualData.item[i] == '1'){
                currentNode = currentNode->right;
            }
            else{
                if(actualData.item[i] == '0'){
                    currentNode = currentNode->left;
                }
            }
        }
    }
}

void decompress(My_string_t fileName){
    FILE *compressedFile = fopen(fileName.item, "rb");

    //File name size
    size_t fileNameSize;
    fread(&fileNameSize, sizeof(size_t), 1, compressedFile);

    //original file name.extension
    My_string_t outputFileName;
    outputFileName.item = malloc(fileNameSize + 1);
    outputFileName.count = fileNameSize;
    outputFileName.capacity = fileNameSize + 1;
    fread(outputFileName.item, sizeof(unsigned char), fileNameSize, compressedFile);
    
    if(compressedFile == NULL){
        printf("File was not opened properly");
        exit(1);
    }
    
    //find bit flag (h/n type)
    uint8_t compressionFlag;
    fread(&compressionFlag, 1, 1, compressedFile);

    if(compressionFlag == 0){

        // huffman decompression
        uint32_t treeLength = 0;
        fread(&treeLength, 1, sizeof(uint32_t), compressedFile); // reading tree length

        Node_t root = {NULL, NULL, {0, 0}};
        rebuildTree(treeLength, compressedFile, &root); // Tree building done

        uint32_t validBit;
        fread(&validBit, 1, sizeof(uint32_t), compressedFile); // reading valid bytes count

        uint32_t encodingSize = (uint32_t)((validBit + 7) / 8);
        uint8_t *encoding = getEncoding(encodingSize, compressedFile);
        
        // Now i Have tree, validByte and encoding
        My_string_t actualData = {NULL, 0, 0};
        decode(encoding, encodingSize, &actualData);// Retreived the actual encoding
        appendNormalStr(&actualData, "\0", 1);

        //actual data is in 010101 for traversal, root is ready
        My_string_t output = {NULL, 0, 0};
        FILE *outputFile = fopen(outputFileName.item, "wb");
    
        generateOutput(&output, validBit, actualData, &root, outputFile);
        printf("Decompression done\n");

        // deleting the compressed file
        for(int i = 0; i < outputFileName.count; i++){
            if(outputFileName.item[i] == '.'){
                outputFileName.count = i+1;
                break;
            }
        }

        appendNormalStr(&outputFileName, "compressed\0", 11);
        printf("File to remove: %s", outputFileName.item);
        remove(outputFileName.item);
        fclose(outputFile);
    }
    else{
        if(compressionFlag == 1){

        }
        else{

        }
        // lzma decompression
    }
    fclose(compressedFile);
    // convert byptes into bits -> iterate till legal count -> traverse tree using the path -> get real bytes -> output 
}

void highCompression(My_string_t fileName){
    //LZMA
    FILE *outputFile;
    fputc('1', outputFile);
}

void calculateFrequencies(My_string_t fileName){
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

int createEncoding(Node_t root, uint8_t target, My_string_t *temp){
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

void bitPacking(My_string_t encoding, FILE *outputFile){
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
    uint32_t validByte = 0;
    My_string_t temp = {NULL, 0, 0};
    My_string_t encoding = {NULL, 0, 0};

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

    validByte = encoding.count;
    fwrite(&validByte, sizeof(uint32_t), 1, outputFile);
    // If encoding is not multiple of 8 then bit packing will not be done
    while(encoding.count % 8 != 0){
        appendChar(&encoding, '0');
    }
    // bit packing
    bitPacking(encoding, outputFile);
}

void storeTree(Node_t *root, FILE *outputFile){
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

void output(My_string_t inputFileName){
    My_string_t rawOutputFileName = {NULL, 0, 0};
    for(int i = 0; i < inputFileName.count; i++){
        if(inputFileName.item[i] == '.'){
            break;
        }
        else{
            appendChar(&rawOutputFileName, inputFileName.item[i]);
        }
    }
    appendNormalStr(&rawOutputFileName, ".compressed\0", 12);

    unsigned char outputFileName[rawOutputFileName.count];

    for(int i = 0; i < rawOutputFileName.count; i++){
        outputFileName[i] = rawOutputFileName.item[i];
    }

    FILE *outputFile = fopen(outputFileName, "wb");

    //length of name.extension
    inputFileName.count--;

    fwrite(&inputFileName.count, sizeof(size_t), 1, outputFile);
    //name.extension
    fwrite(inputFileName.item, sizeof(unsigned char), inputFileName.count, outputFile);
    // Flag of huffman compression type
    fputc((uint8_t)0, outputFile);

    uint32_t treeLength = (uint32_t)(frequency_array.count*2 + frequency_array.count - 1);
    fwrite(&treeLength, sizeof(uint32_t), 1, outputFile); // Size of tree buffer - just iterate till this count and you get your ccomplete tree 
    storeTree(&tree_array.node[0], outputFile); // serialization
    // printTree(tree_array.node[0]); // printing after building a tree during compression
    createDataBuffer(outputFile);

    appendNormalStr(&inputFileName, "\0", 1);
    printf("Removing during compression: %s\n", inputFileName.item);
    remove(inputFileName.item);    
    fclose(outputFile);
}

void huffmanCompression(My_string_t fileName){
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