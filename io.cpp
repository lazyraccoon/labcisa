#include<iostream>
#include<string>
#include<limits>
#include<fstream>

#include "io.h"

std::string readString(std::string text){
    std::cout << text;
    std::string result;
    std::cin.clear();
    std::cin.sync();
    std::getline(std::cin, result);
    return result;
}

bool readBooleanFile(std::ifstream &file){
    int number;
    file >> number;
    return number == 1;
}

int readNumber(std::string text){
    std::cout << text;
    int result;
    while(!(std::cin >> result)){
        std::cin.clear();
        std::cout << "Bad value! Try again: ";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return result;
}

bool readBoolean(std::string text){
    std::cout << text;
    int result;
    while(1){
        int result = readNumber("");
        if(result == 1)
            return true;
        if(result == 0)
            return false;
    }
}
