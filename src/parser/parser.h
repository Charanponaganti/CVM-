#pragma once
#include "../lexer/Token.h"
#include "AST.h"
#include "Stmt.h"
#include <vector>
#include <memory>
#include <stdexcept>

class Parser {
private:
    std::vector<Token> tokens;
    int current = 0;

public:
    Parser(const std::vector<Token>& t);

    std::vector<std::unique_ptr<Stmt>> parse();

private:
    Token peek();
    Token previous();
    Token advance();
    bool isAtEnd();
    bool check(TokenType type);
    bool match(std::initializer_list<TokenType> types);
    Token consume(TokenType type, const std::string& message);

    std::unique_ptr<Stmt> declaration();
    std::unique_ptr<Stmt> letDeclaration();
    std::unique_ptr<Stmt> statement();
    std::unique_ptr<Stmt> printStatement();
    std::unique_ptr<Stmt> ifStatement();
    std::unique_ptr<Stmt> whileStatement();
    std::unique_ptr<Stmt> expressionStatement();
    std::vector<std::unique_ptr<Stmt>> block();

    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> assignment();
    std::unique_ptr<Expr> equality();
    std::unique_ptr<Expr> comparison();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();
    std::unique_ptr<Expr> primary();
};