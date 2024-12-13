#ifndef SOUSE_H
#define SOUSE_H

#include <string>

class Souse {
private:
    bool hadError_ = false;

    void run(const std::string& source);
    void report(int line, const std::string& where, const std::string& message);

public:
    void runPrompt();
    void runFile(const std::string& path);
    void error(int line, const std::string& message);
};

#endif 