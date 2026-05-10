#include "Parser.h"
#include <cstdlib>



Parser::Parser(const std::vector<Token>& t)
    : tokens(t) {}

bool Parser::isAtEnd() {
    return peek().type == TokenType::EOF_TOKEN;
}

Token Parser::peek() {
    return tokens[current];
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return tokens[current - 1];
}
std::unique_ptr<Expr> Parser::parse() {
    return expression();
}
std::unique_ptr<Expr> Parser::expression() {
    auto expr = term();

    while (
        peek().type == TokenType::PLUS ||
        peek().type == TokenType::MINUS
    ) {
        Token op = advance();

        auto right = term();

        expr = std::make_unique<BinaryExpr>(
            std::move(expr),
            op,
            std::move(right)
        );
    }

    return expr;
}
std::unique_ptr<Expr> Parser::term() {
    auto expr = factor();

    while (
        peek().type == TokenType::STAR ||
        peek().type == TokenType::SLASH
    ) {
        Token op = advance();

        auto right = factor();

        expr = std::make_unique<BinaryExpr>(
            std::move(expr),
            op,
            std::move(right)
        );
    }

    return expr;
}
std::unique_ptr<Expr> Parser::factor() {
    return primary();
}


