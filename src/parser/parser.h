#pragma once
#include "../lexer/Token.h"
#include "AST.h"
#include <vector>

class Parser {
private:
    std::vector<Token> tokens;
    int current = 0;

public:
    Parser(const std::vector<Token>& t);

    std::unique_ptr<Expr> parse();

private:
    Token peek();
    Token advance();
    bool isAtEnd();

    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();
    std::unique_ptr<Expr> primary();
};