// src/main.cpp
#include "../include/souse.h"
#include <iostream>

int main(int argc, char* argv[]) {
    Souse interpreter;

    try {
        if(argc > 2) {
            std::cerr << "Usage: souse [script]" << std::endl;
            return 64;
        }
        else if(argc == 2) {
            interpreter.runFile(std::string(argv[1]));
        }
        else {
            interpreter.runPrompt();
        }
    }
    catch(const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}