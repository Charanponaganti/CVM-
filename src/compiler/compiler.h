#pragma once

#include "opcode.h"
#include "../parser/AST.h"
#include "../parser/Stmt.h"
#include <vector>
#include <memory>

class Compiler {
private:
    Chunk chunk;

public:
    Chunk compile(const std::vector<std::unique_ptr<Stmt>>& statements);

    // Debug: disassemble the bytecode
    void disassemble(const Chunk& chunk);

private:
    void compileStmt(Stmt* stmt);
    void compileExpr(Expr* expr);

    // Helpers for jump patching
    int emitJump(OpCode op);
    void patchJump(int offset);
};
