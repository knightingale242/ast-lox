#include <iostream>

#include "token.h"

class Parser{
private:
    std::vector<Token> tokens_;
    int current_ = 0;

    Expr expression(){
        return equality();
    }
public:
    Parser(std::vector<Token>& tokens)
        : tokens_(tokens)
    {} 
};