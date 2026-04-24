#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper.h"

// LZMA - I was thinking of implementing it but I guess I'll be droping this idea as I am lacking of time
// As a future upgrade I can implement lzma or lz77 for better compression
// Still the current version may support further development for lz without big changes

// Documentation:

// Basic info:
// 2 modes for data compression 
// - Huffman algorithm for normal data compression (flag = 0)
// - LZMA Algorithm for high compresssion (flag = 1) 

// Output format:
// length of name (size_t) + name.extension (char*) + type flag('0':Huffman/ '1':LZMA)(char) + Tree length (n uint32) + tree data (serialized) + Valid count (uint32) + Encoding

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

// - This file is only a CLI router. It validates user arguments and forwards work to helper.h.
// - Actual compression and decompression pipelines are implemented in helper.h.
// - Core data structures used by helper.h live in src/DataStructures/:
//   data_array.h      -> stores the raw bytes read from input file
//   frequency_array.h -> stores byte frequency table
//   tree_array.h      -> builds/merges Huffman nodes into a tree
//   my_string.h       -> custom dynamic string used across command handlers
// - Current status: Huffman compression/decompression is implemented, high compression is a placeholder.

// Main function for accepting user arguments
int main(int argc, char* argv[]){

    // Case A: informational commands without file IO.
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
        else{
            error();
        }
    }
    // Case B: compression command with mode + input file.
    else if(argc == 4){
        if(strcmp(argv[1], "-c") == 0){

            // Helper functions accept My_string_t, so convert argv[3] to project string type.
            My_string_t fileName = {NULL, 0, 0};
            size_t argvLength = strlen(argv[3]);

            for(int i = 0; i < argvLength; i++){
                appendChar(&fileName, argv[3][i]);
            }
            appendChar(&fileName, *"\0");

            // -h currently calls placeholder highCompression(), kept for future LZ support.
            if(strcmp(argv[2], "-h") == 0){
                highCompression(fileName); // Not implemented
            }
            // -n runs fully implemented Huffman compression pipeline.
            else if(strcmp(argv[2], "-n") == 0){
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
        // Case C: decompression command with compressed file.
        if(argc == 3){
            if(strcmp(argv[1], "-d") == 0){
                My_string_t fileName = {NULL, 0, 0};
                size_t argvLength = strlen(argv[2]);
                
                for(int i = 0; i < argvLength; i++){
                    appendChar(&fileName, argv[2][i]);
                }
                appendChar(&fileName, *"\0");
                // decompress() reads file header first, then routes by compression flag.
                decompress(fileName);
            }
            else{
                error();
            }
        }
        else{
            // Guard branch for all unsupported argument counts/patterns.
            error();
        }
    }

    return 0;
}