#ifndef SOUSE_SCANNER_H
#define SOUSE_SCANNER_H

#include <string>
#include <vector>
#include <unordered_map>
#include "token.h"

class Scanner {
private:
    std::string source_;
    std::vector<Token> tokens;
    int start_ = 0;
    int current_ = 0;
    int line_ = 1;

    bool isAtEnd();
    char peek();
    char peekNext();
    void addToken(TokenType type, std::any literal);
    void addToken(TokenType type);
    char advance();
    bool match(char expected);
    void string();
    bool isDigit(char c);
    bool isAlpha(char c);
    void identifier();
    void number();
    void scanToken();

public:
    Scanner(std::string source);
    std::vector<Token> scanTokens();
};

#endif