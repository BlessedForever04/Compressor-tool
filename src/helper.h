#include <stdio.h>

void help(){
    printf("\nCurrent commands in use :\n\n");
    printf("Fast file compression       ./compressor -c -f file_name.extension\n");
    printf("Normal file compression     ./compressor -c -n file_name.extension\n");
    printf("Decompress file             ./compressor -d file_name.extension\n");
    printf("About                       ./compressor -about\n");
    printf("GitHub repo                 ./compressor -github\n");
    printf("\nFor more information about tool, checkout github repo.\n\n");
}

void github(){
    system("start \"\" \"https://github.com/BlessedForever04/Compressor-tool\"");
}

void about(){
    
}