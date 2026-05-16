#pragma once

#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include "AST.h"

struct Stmt {
    virtual ~Stmt() = default;
    virtual void dump(int indent = 0) = 0;
};

struct ExprStmt : Stmt {
    std::unique_ptr<Expr> expression;

    ExprStmt(std::unique_ptr<Expr> expr) : expression(std::move(expr)) {}

    void dump(int indent = 0) override {
        std::cout << std::string(indent, ' ') << "ExprStmt\n";
        if (expression) expression->dump(indent + 2);
    }
};

struct PrintStmt : Stmt {
    std::unique_ptr<Expr> expression;

    PrintStmt(std::unique_ptr<Expr> expr) : expression(std::move(expr)) {}

    void dump(int indent = 0) override {
        std::cout << std::string(indent, ' ') << "PrintStmt\n";
        if (expression) expression->dump(indent + 2);
    }
};

struct LetStmt : Stmt {
    Token name;
    std::unique_ptr<Expr> initializer;

    LetStmt(Token n, std::unique_ptr<Expr> init) : name(n), initializer(std::move(init)) {}

    void dump(int indent = 0) override {
        std::cout << std::string(indent, ' ') << "LetStmt(" << name.lexeme << ")\n";
        if (initializer) {
            initializer->dump(indent + 2);
        }
    }
};

struct BlockStmt : Stmt {
    std::vector<std::unique_ptr<Stmt>> statements;

    BlockStmt(std::vector<std::unique_ptr<Stmt>> stmts) : statements(std::move(stmts)) {}

    void dump(int indent = 0) override {
        std::cout << std::string(indent, ' ') << "BlockStmt\n";
        for (const auto& stmt : statements) {
            stmt->dump(indent + 2);
        }
    }
};

struct IfStmt : Stmt {
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> thenBranch;
    std::unique_ptr<Stmt> elseBranch;

    IfStmt(std::unique_ptr<Expr> cond, std::unique_ptr<Stmt> thenB, std::unique_ptr<Stmt> elseB)
        : condition(std::move(cond)), thenBranch(std::move(thenB)), elseBranch(std::move(elseB)) {}

    void dump(int indent = 0) override {
        std::cout << std::string(indent, ' ') << "IfStmt\n";
        if (condition) condition->dump(indent + 2);
        if (thenBranch) thenBranch->dump(indent + 2);
        if (elseBranch) {
            std::cout << std::string(indent, ' ') << "ElseBranch\n";
            elseBranch->dump(indent + 2);
        }
    }
};

struct WhileStmt : Stmt {
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> body;

    WhileStmt(std::unique_ptr<Expr> cond, std::unique_ptr<Stmt> b)
        : condition(std::move(cond)), body(std::move(b)) {}

    void dump(int indent = 0) override {
        std::cout << std::string(indent, ' ') << "WhileStmt\n";
        if (condition) condition->dump(indent + 2);
        if (body) body->dump(indent + 2);
    }
};
