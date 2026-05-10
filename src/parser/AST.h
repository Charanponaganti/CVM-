//abstratc syntax tree

#pragma once

#include <memory>
#include <iostream>


#include "../lexer/Token.h"
struct Expr {
    virtual ~Expr() = default;

    virtual void dump(int indent = 0) = 0;
};

struct NumberExpr : Expr {
    double value;

    NumberExpr(double val)
        : value(val) {}

    void dump(int indent = 0) override {
        std::cout
            << std::string(indent, ' ')
            << "Number(" << value << ")\n";
    }
};
struct BinaryExpr : Expr {
    std::unique_ptr<Expr> left;
    Token op;
    std::unique_ptr<Expr> right;

    BinaryExpr(
        std::unique_ptr<Expr> l,
        Token oper,
        std::unique_ptr<Expr> r
    )
        : left(std::move(l)),
          op(oper),
          right(std::move(r)) {}

    void dump(int indent = 0) override {
        std::cout
            << std::string(indent, ' ')
            << "Binary(" << op.lexeme << ")\n";

        left->dump(indent + 2);
        right->dump(indent + 2);
    }
};

