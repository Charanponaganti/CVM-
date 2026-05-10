#include "Lexer.h"
#include <cctype>

Lexer::Lexer(const std::string& src)
    : source(src) {}

bool Lexer::isAtEnd() {
    return current >= source.length();
}

char Lexer::advance() {
    return source[current++];
}

char Lexer::peek() {
    if (isAtEnd()) return '\0';
    return source[current];
}

void Lexer::addToken(TokenType type, const std::string& lexeme) {
    tokens.emplace_back(type, lexeme);
}
std::vector<Token> Lexer::tokenize() {
    while (!isAtEnd()) {
        scanToken();
    }

    addToken(TokenType::EOF_TOKEN, "");
    return tokens;
}
void Lexer::scanToken() {
    char c = advance();

    switch (c) {
        case '+':
            addToken(TokenType::PLUS, "+");
            break;

        case '-':
            addToken(TokenType::MINUS, "-");
            break;

        case '*':
            addToken(TokenType::STAR, "*");
            break;

        case '/':
            addToken(TokenType::SLASH, "/");
            break;

        case '=':
            addToken(TokenType::EQUAL, "=");
            break;

        case '(':
            addToken(TokenType::LEFT_PAREN, "(");
            break;

        case ')':
            addToken(TokenType::RIGHT_PAREN, ")");
            break;

        case ' ':
        case '\r':
        case '\t':
        case '\n':
            break;

        default:
            if (isdigit(c)) {
                current--;
                number();
            }
            else if (isalpha(c)) {
                current--;
                identifier();
            }
            break;
    }
}
void Lexer::number() {
    std::string value;

    while (isdigit(peek())) {
        value += advance();
    }

    addToken(TokenType::NUMBER, value);
}
void Lexer::identifier() {
    std::string value;

    while (isalnum(peek())) {
        value += advance();
    }

    if (value == "let")
        addToken(TokenType::LET, value);

    else if (value == "print")
        addToken(TokenType::PRINT, value);

    else
        addToken(TokenType::IDENTIFIER, value);
}