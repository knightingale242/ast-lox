#include "../include/souse.h"
#include "../include/scanner.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

void Souse::run(const std::string& source) {
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.scanTokens();

    for (const Token& token : tokens) {
        std::cout << token.toString() << std::endl;
    }
}

void Souse::report(int line, const std::string& where, const std::string& message) {
    std::cerr << "[line " << line << "] Error" << where << ": " << message << std::endl;
    hadError_ = true;
}

void Souse::runPrompt() {
    std::string input;
    std::cout << "Souse REPL (type 'exit' to quit)\n";
    
    while(true) {
        std::cout << "> ";
        std::getline(std::cin, input);
        
        if(input == "exit") {
            break;
        }
        
        run(input);
        hadError_ = false;
    }
}

void Souse::runFile(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    
    if(!file.is_open()) {
        std::cerr << "Error opening the file: " << path << std::endl;
        std::exit(65);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();

    run(source);

    if(hadError_) {
        std::exit(65);
    }
}

void Souse::error(int line, const std::string& message) {
    report(line, "", message);
}