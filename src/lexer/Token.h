#pragma once

#include <string>
using namespace std;


enum class TokenType {
    //single chars
    PLUS,
    MINUS,STAR,SLASH,EQUAL,
    LEFT_PAREN,RIGHT_PAREN,

    //identiff
    NUMBER,IDENTIFIER,
    //keyworkds
    LET,PRINT,IF,ELSE,WHILE,TRUE,FALSE,

    EOF_TOKEN

};

struct Token{
    TokenType type;
    string lexeme;

    Token(TokenType t,const string &l)
        :type(t), lexeme(l) {}

};
