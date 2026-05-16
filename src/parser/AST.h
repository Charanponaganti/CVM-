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
struct BooleanExpr : Expr {
    bool value;

    BooleanExpr(bool val) : value(val) {}

    void dump(int indent = 0) override {
        std::cout << std::string(indent, ' ') << "Boolean(" << (value ? "true" : "false") << ")\n";
    }
};

struct GroupingExpr : Expr {
    std::unique_ptr<Expr> expression;

    GroupingExpr(std::unique_ptr<Expr> expr) : expression(std::move(expr)) {}

    void dump(int indent = 0) override {
        std::cout << std::string(indent, ' ') << "Grouping\n";
        if (expression) expression->dump(indent + 2);
    }
};

struct VariableExpr : Expr {
    Token name;

    VariableExpr(Token n) : name(n) {}

    void dump(int indent = 0) override {
        std::cout << std::string(indent, ' ') << "Variable(" << name.lexeme << ")\n";
    }
};

struct AssignExpr : Expr {
    Token name;
    std::unique_ptr<Expr> value;

    AssignExpr(Token n, std::unique_ptr<Expr> v) : name(n), value(std::move(v)) {}

    void dump(int indent = 0) override {
        std::cout << std::string(indent, ' ') << "Assign(" << name.lexeme << ")\n";
        if (value) value->dump(indent + 2);
    }
};

struct InputExpr : Expr {
    InputExpr() {}

    void dump(int indent = 0) override {
        std::cout << std::string(indent, ' ') << "Input\n";
    }
};
