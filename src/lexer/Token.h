#pragma once

#include <string>
using namespace std;


enum class TokenType {
    //single chars
    PLUS,
    MINUS,STAR,SLASH,EQUAL,
    LEFT_PAREN,RIGHT_PAREN,LESS_THAN,LEFT_BRACE,RIGHT_BRACE,

    //two chars
    EQUAL_EQUAL,

    //identiff
    NUMBER,IDENTIFIER,
    //keyworkds
    LET,PRINT,IF,ELSE,WHILE,TRUE,FALSE,INPUT,

    EOF_TOKEN

};

struct Token{
    TokenType type;
    string lexeme;

    Token(TokenType t,const string &l)
        :type(t), lexeme(l) {}

};
