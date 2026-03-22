#include <iostream>
#include <string>

std::string getBinary(char c){
    std::string binary = "";

    for(int i = 7; i >= 0; i--){
        binary += (c >> i) & 1;
    }

    return binary;
}

void decode(std::string input){
    std::string binary = "";
    for(int i = 0; i < input.size(); i++){
        binary += getBinary(input[i]).substr(2, 6);
    }

    for(int i = 0; i < binary.size() / 8; i++){
        for(int j = 0; j < 8; j++){
            
        }
    }
}