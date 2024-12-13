#include "../include/scanner.h"
#include <cctype>
#include <iostream>

std::unordered_map<std::string, TokenType> Scanner::keywords_ = {
    {"and", AND},
    {"class", CLASS},
    {"else", ELSE},
    {"false", FALSE},
    {"for", FOR},
    {"fun", FUN},
    {"if", IF},
    {"nil", NIL},
    {"or", OR},
    {"print", PRINT},
    {"return", RETURN},
    {"super", SUPER},
    {"this", THIS},
    {"true", TRUE},
    {"var", VAR},
    {"while", WHILE}

};

Scanner::Scanner(std::string source) : source_(source) {}

bool Scanner::isAtEnd() {
    return current_ >= source_.length();
}

char Scanner::peek() {
    if(isAtEnd()) return '\0';
    return source_.at(current_);
}

char Scanner::peekNext() {
    if (current_ + 1 >= source_.length()) return '\0';
    return source_.at(current_ + 1);
}

void Scanner::addToken(TokenType type, std::any literal) {
    std::string text = source_.substr(start_, current_ - start_);
    tokens_.push_back(Token(type, text, literal, line_));
}

void Scanner::addToken(TokenType type) {
    addToken(type, nullptr);
}

char Scanner::advance() {
    return source_.at(current_++);
}

bool Scanner::match(char expected) {
    if(isAtEnd()) return false;
    if(source_.at(current_) != expected) return false;
    current_++;
    return true;
}

void Scanner::string() {
    while(peek() != '"' && !isAtEnd()) {
        if(peek() == '\n') line_++;
        advance();
    }

    if(isAtEnd()) {
        // TODO: Proper error handling
        return;
    }

    advance(); // Consume closing quote

    std::string value = source_.substr(start_ + 1, current_ - start_ - 2);
    addToken(STRING, value);
}

bool Scanner::isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool Scanner::isAlpha(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
}

void Scanner::identifier() {
    while (std::isalnum(peek()) || peek() == '_') {
        advance();
    }
    
    std::string text = source_.substr(start_, current_ - start_);
    TokenType type = (keywords_.count(text) > 0) ? keywords_[text] : IDENTIFIER; 
    addToken(type);
}

void Scanner::number() {
    while(isDigit(peek())) {
        advance();
    }

    // Look for fractional part
    if (peek() == '.' && isDigit(peekNext())) {
        advance(); // Consume the '.'

        while(isDigit(peek())) {
            advance();
        }
    }

    double value = std::stod(source_.substr(start_, current_ - start_));
    addToken(NUMBER, value);
}

void Scanner::scanToken() {
    char c = advance();
    switch(c) {
        case '(': addToken(LEFT_PAREN); break;
        case ')': addToken(RIGHT_PAREN); break;
        case '{': addToken(LEFT_BRACE); break;
        case '}': addToken(RIGHT_BRACE); break;
        case ',': addToken(COMMA); break;
        case '.': addToken(DOT); break;
        case '-': addToken(MINUS); break;
        case '+': addToken(PLUS); break;
        case ';': addToken(SEMICOLON); break;
        case '*': addToken(STAR); break;
        
        case '!': 
            addToken(match('=') ? BANG_EQUAL : BANG);
            break;
        case '=': 
            addToken(match('=') ? EQUAL_EQUAL : EQUAL);
            break;
        case '<': 
            addToken(match('=') ? LESS_EQUAL : LESS);
            break;
        case '>': 
            addToken(match('=') ? GREATER_EQUAL : GREATER);
            break;
        
        case '/':
            if (match('/')) {
                // A comment goes until the end of the line
                while (peek() != '\n' && !isAtEnd()) advance();
            } else {
                addToken(SLASH);
            }
            break;
        
        case ' ':
        case '\r':
        case '\t':
            break;
        
        case '\n':
            line_++;
            break;
        
        case '"': 
            string(); 
            break;
        
        default:
            if (isDigit(c)) {
                number();
            } else if (isAlpha(c)) {
                identifier();
            } else {
                // TODO: Proper error handling for unexpected characters
                std::cerr << "Unexpected character at line " << line_ << std::endl;
            }
            break;
    }
}

std::vector<Token> Scanner::scanTokens() {
    while (!isAtEnd()) {
        start_ = current_;
        scanToken();
    }

    tokens_.push_back(Token(EOFT, "", nullptr, line_));
    return tokens_;
}