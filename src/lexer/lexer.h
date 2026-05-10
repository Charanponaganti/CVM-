#pragma once
#include "Token.h"
#include <vector>

class Lexer {
private:
    std::string source;
    std::vector<Token> tokens;
    int current = 0;

public:
    Lexer(const std::string& src);

    std::vector<Token> tokenize();

private:
    bool isAtEnd();
    char advance();
    char peek();

    void scanToken();

    void number();
    void identifier();

    void addToken(TokenType type, const std::string& lexeme);
};