#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <any>
#include <cctype>

enum TokenType{
    //Single-character tokens
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    COMMA,
    DOT,
    MINUS,
    PLUS,
    SEMICOLON,
    SLASH,
    STAR,
    //one or two character tokens
    BANG,
    BANG_EQUAL,
    EQUAL,
    EQUAL_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,
    //literals
    IDENTIFIER,
    STRING,
    NUMBER,
    //keywords
    AND,
    CLASS,
    ELSE,
    FALSE,
    FUN,
    FOR,
    IF,
    NIL,
    OR,
    PRINT,
    RETURN,
    SUPER,
    THIS,
    TRUE,
    VAR,
    WHILE,
    EOFT
};

class Token{
private:
public:
    TokenType type_;
    std::string lexeme_;
    std::any literal_;
    int line_;

    Token(TokenType type, std::string lexeme, std::any literal, int line)
        : type_(type)
        , lexeme_(lexeme)
        , literal_(literal)
        , line_(line)
    { }

    std::string toString(){
        // return type_ + " " + lexeme_ + " " + literal_;
        return "";
    }
};

class Scanner{
private:
    std::string source_;
    std::vector<Token> tokens;
    std::unordered_map<std::string, TokenType> keywords_;
    int start_ = 0;
    int current_ = 0;
    int line_ = 1;

    bool isAtEnd(){
        return current_ >= source_.length();
    }

    char peek(){
        if(isAtEnd()){
            return '\0';
        }
        return source_.at(current_);
    }

    char peekNext(){
        if (current_ + 1 >= source_.length()){
            return '\0';
        }
        return source_.at(current_ + 1);
    }

    void addToken(TokenType type, std::any literal){
        std::string text = source_.substr(start_, current_);
        tokens.push_back(Token(type, text, literal, line_));
    }

    void addToken(TokenType type){
        addToken(type, nullptr);
    }

    char advance(){
        return source_.at(current_++);
    }

    bool match(char expected){
        if(isAtEnd()){
            return false;
        }
        if(source_.at(current_) != expected){
            return false;
        }
        current_++;
        return true;
    }

    void string(){
        while(peek() != '"' && !isAtEnd()){
            if(peek() == '\n'){
                line_++;
            }
            advance();
        }

        if(isAtEnd()){
            //TODO throw a souse error call here
            return;
        }

        advance(); //get the closing "

        std::string value = source_.substr(start_ + 1, current_ + 1);
        addToken(STRING, value);
    }

    bool isDigit(char c){
        return c >= '0' && c <= '9';
    }

    bool isAlpha(char c){
        return (c >= 'a' && c <= 'z') ||
               (c >= 'A' && c <= 'Z') ||
               c == '_';
    }

    void identifier(){
        while (std::isalnum(peek()) && !isAtEnd()){
            advance();
        }
        addToken(IDENTIFIER, source_.substr(start_, current_));
    }

    void number(){
        while(isDigit(peek()) && !isAtEnd()){
            advance();
        }

        if(peek() == '.' && isDigit(peekNext())){
            advance();
        }

        while(isDigit(peek()) && !isAtEnd()){
            advance();
        }

        addToken(NUMBER, std::stod(source_.substr(start_, current_)));
    }

    void scanToken(){
        char c = advance();
        switch(c){
            case '(':{
                addToken(LEFT_PAREN);
                break;
            }
            case ')':{
                addToken(RIGHT_PAREN);
                break;
            }
            case '{':{
                addToken(LEFT_BRACE);
                break;
            }
            case '}':{
                addToken(RIGHT_BRACE);
            }
            case ',':{
                addToken(COMMA);
                break;
            }
            case '.':{
                addToken(DOT);
                break;
            }
            case '-':{
                addToken(MINUS);
                break;
            }
            case '+':{
                addToken(PLUS);
                break;
            }
            case ';':{
                addToken(SEMICOLON);
                break;
            }
            case '*':{
                addToken(STAR);
                break;
            }
            case '!':{
                addToken(match('=') ? BANG_EQUAL : BANG);
                break;
            }
            case '=':{
                addToken(match('=') ? EQUAL_EQUAL : EQUAL);
                break;
            }
            case '<':{
                addToken(match('=') ? LESS_EQUAL : LESS);
                break;
            }
            case '>':{
                addToken(match('=') ? GREATER_EQUAL : GREATER);
            }
            case '/':{
                if(match('/')){
                    while(peek() != '\n' && !isAtEnd()){
                        advance();
                    }
                }
                else{
                    addToken(SLASH);
                }
                break;
            }
            case ' ':
            case '\t':
            case '\r':
                break;
            case '\n':
                line_++;
                break;
            case '"':
                string();
                break;
            default:{
                if(isDigit(c)){
                    number();
                }
                else if(isAlpha(c)){
                    identifier();
                }
                else{
                    //TODO throw a souse error call here
                }
                break;
            }
        }
    }

public:
    Scanner(std::string source)
        : source_(source)
    {
        
    }

    std::vector<Token> scanTokens(){
        while(!isAtEnd()){
            start_ = current_;
            scanToken();
        }

        tokens.push_back(Token(EOFT, "", nullptr, line_));
        return tokens;
    }
};

class Souse {
private:
    bool hadError_ = false;

    void run(const std::string& source) {
        Scanner scanner(source);
        std::vector<Token> tokens = scanner.scanTokens();

        for (const Token& token : tokens) {
            // std::cout << token << std::endl;
        }
    }

    void report(int line, const std::string& where, const std::string& message) {
        std::cerr << "[line " << line << "] Error" << where << ": " << message << std::endl;
        hadError_ = true;
    }

public:
    void runPrompt() {
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

    void runFile(const std::string& path) {
        std::ifstream file(path, std::ios::binary);
        
        if(!file.is_open()) {
            std::cerr << "Error opening the file: " << path << std::endl;
            std::exit(65);
        }

        file.seekg(0, std::ios::end);
        std::streamsize file_size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<char> buffer(file_size);
        
        if(!file.read(buffer.data(), file_size)) {
            throw std::ios_base::failure("Failed to read the file");
        }

        run(std::string(buffer.begin(), buffer.end()));

        if(hadError_) {
            std::exit(65);
        }
    }

    void error(int line, const std::string& message) {
        report(line, "", message);
    }
};

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