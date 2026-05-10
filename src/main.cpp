#include "lexer/lexer.h"
#include <iostream>

std::string tokenTypeToString(TokenType type) {
    switch(type) {
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::STAR: return "STAR";
        case TokenType::SLASH: return "SLASH";
        case TokenType::EQUAL: return "EQUAL";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::LET: return "LET";
        case TokenType::PRINT: return "PRINT";
        case TokenType::EOF_TOKEN: return "EOF";
        default: return "UNKNOWN";
    }
}

int main() {
    std::string code = "let x = 10 + 5";

    Lexer lexer(code);

    auto tokens = lexer.tokenize();

    for (const auto& token : tokens) {
        std::cout
            << tokenTypeToString(token.type)
            << " : "
            << token.lexeme
            << std::endl;
    }

    return 0;
}