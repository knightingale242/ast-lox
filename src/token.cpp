#include "../include/token.h"

Token::Token(TokenType type, std::string lexeme, std::any literal, int line)
    : type_(type), lexeme_(lexeme), literal_(literal), line_(line) {}

std::string Token::toString() const {
    // Placeholder implementation
    return "Token: " + lexeme_;
}