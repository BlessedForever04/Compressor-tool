#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper.h"

// Read bytes -> get its frequency -> build huffman tree -> generate code (0 / 1) -> bit packing and legal count -> output
// convert byptes into bits -> iterate till legal count -> traverse tree using the path -> get real bytes -> output 
// Huffman algorithm -> Normal speed compression
// LZMA algorithm -> High speed cmopression

int main(int argc, char* argv[]){

    if (argc == 2) {
        if (strcmp(argv[1], "help") == 0 || strcmp(argv[1], "-help") == 0) {
            help();
        }
        else if (strcmp(argv[1], "-github") == 0) {
            github();
        }
        else if (strcmp(argv[1], "-about") == 0) {
            about();
        }
    }

    return 0;
}