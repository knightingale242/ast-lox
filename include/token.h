#ifndef SOUSE_TOKEN_H
#define SOUSE_TOKEN_H

#include <string>
#include <any>
#include "token_type.h"

class Token {
public:
    TokenType type_;
    std::string lexeme_;
    std::any literal_;
    int line_;

    Token(TokenType type, std::string lexeme, std::any literal, int line);
    std::string toString() const;
};

#endif 