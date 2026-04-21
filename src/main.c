#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper.h"

// Documentation:

// Basic info:
// 2 modes for data compression 
// - Huffman algorithm for normal data compression (flag = 0)
// - LZMA Algorithm for high compresssion (flag = 1)

// Using bit flags for classification of compression type
// - 0 for Huffman : 1 for LZMA

// Serialization of tree
// - 1 if no children found and 0 if it has children
// - example : 0 0 1 75 1 89
// - Check helper.h serializeTree() for more clarification

// Algorithm for Huffman
// - Compression: Read bytes -> get its frequency (stored in frequency_array) -> Build huffman tree (stored in tree_array) -> store flag + tree size + serializeTree in output -> create encoding (0/1) -> Bit packing and legal count -> output
// - Decompression: Read flag -> get tree size -> iterate(tree size) and deserializeTree -> unpack bits -> get output



// Read bytes -> get its frequency -> build huffman tree -> generate code (0 / 1) -> bit packing and legal count -> output
// convert byptes into bits -> iterate till legal count -> traverse tree using the path -> get real bytes -> output 
// Huffman algorithm -> Normal compression
// LZMA algorithm -> High cmopression
// Add multi threading to make it fun (wherever its possbile);

// run length encoding can be used before implementing huffman tree to compress more - experiment
int main(int argc, char* argv[]){

    if(argc == 2){
        if (strcmp(argv[1], "help") == 0 || strcmp(argv[1], "-help") == 0) {
            help();
        }
        else if (strcmp(argv[1], "-github") == 0) {
            github();
        }
        else if (strcmp(argv[1], "-about") == 0) {
            about();
        }
        else if(strcmp(argv[1], "-d") == 0){
            decompress();
        }
        else{
            error();
        }
    }
    else if(argc == 4){
        if(strcmp(argv[1], "-c") == 0){
            if(strcmp(argv[2], "-h") == 0){
                highCompression();
            }
            else if(strcmp(argv[2], "-n") == 0){
                struct My_string fileName = {NULL, 0, 0};
                size_t argvLength = strlen(argv[3]);

                for(int i = 0; i < argvLength; i++){
                    appendChar(&fileName, argv[3][i]);
                }
                appendChar(&fileName, *"\0");
                huffmanCompression(fileName);
            }
            else{
                error();
            }
        }
        else{
            error();
        }
    }
    else{
        error();
    }

    return 0;
}