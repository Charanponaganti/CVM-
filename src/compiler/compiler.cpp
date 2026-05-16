#include "compiler.h"
#include <iostream>
#include <iomanip>

Chunk Compiler::compile(const std::vector<std::unique_ptr<Stmt>>& statements) {
    chunk = Chunk(); // reset

    for (const auto& stmt : statements) {
        compileStmt(stmt.get());
    }

    chunk.write(OpCode::OP_HALT);
    return chunk;
}

void Compiler::compileStmt(Stmt* stmt) {
    if (auto* exprStmt = dynamic_cast<ExprStmt*>(stmt)) {
        compileExpr(exprStmt->expression.get());
        chunk.write(OpCode::OP_POP); // discard result
    }
    else if (auto* printStmt = dynamic_cast<PrintStmt*>(stmt)) {
        compileExpr(printStmt->expression.get());
        chunk.write(OpCode::OP_PRINT);
    }
    else if (auto* letStmt = dynamic_cast<LetStmt*>(stmt)) {
        if (letStmt->initializer) {
            compileExpr(letStmt->initializer.get());
        } else {
            // Default to 0 if no initializer
            uint8_t idx = chunk.addConstant(0);
            chunk.write(OpCode::OP_CONST);
            chunk.write(idx);
        }
        uint8_t nameIdx = chunk.addName(letStmt->name.lexeme);
        chunk.write(OpCode::OP_SET_GLOBAL);
        chunk.write(nameIdx);
        chunk.write(OpCode::OP_POP); // discard; let is a statement
    }
    else if (auto* blockStmt = dynamic_cast<BlockStmt*>(stmt)) {
        for (const auto& s : blockStmt->statements) {
            compileStmt(s.get());
        }
    }
    else if (auto* ifStmt = dynamic_cast<IfStmt*>(stmt)) {
        // Compile condition
        compileExpr(ifStmt->condition.get());

        // Emit jump-if-false to else branch
        int thenJump = emitJump(OpCode::OP_JUMP_IF_FALSE);

        // Compile then branch
        compileStmt(ifStmt->thenBranch.get());

        // Emit jump over else branch
        int elseJump = emitJump(OpCode::OP_JUMP);

        // Patch the then-jump to land here (start of else)
        patchJump(thenJump);

        // Compile else branch if present
        if (ifStmt->elseBranch) {
            compileStmt(ifStmt->elseBranch.get());
        }

        // Patch the else-jump to land here (after else)
        patchJump(elseJump);
    }
    else if (auto* whileStmt = dynamic_cast<WhileStmt*>(stmt)) {
        // Record the top of the loop
        int loopStart = static_cast<int>(chunk.code.size());

        // Compile condition
        compileExpr(whileStmt->condition.get());

        // Jump out if false
        int exitJump = emitJump(OpCode::OP_JUMP_IF_FALSE);

        // Compile body
        compileStmt(whileStmt->body.get());

        // Jump back to loop start
        chunk.write(OpCode::OP_JUMP);
        // Encode loopStart as two bytes
        chunk.write(static_cast<uint8_t>((loopStart >> 8) & 0xFF));
        chunk.write(static_cast<uint8_t>(loopStart & 0xFF));

        // Patch exit jump
        patchJump(exitJump);
    }
}

void Compiler::compileExpr(Expr* expr) {
    if (auto* num = dynamic_cast<NumberExpr*>(expr)) {
        uint8_t idx = chunk.addConstant(num->value);
        chunk.write(OpCode::OP_CONST);
        chunk.write(idx);
    }
    else if (auto* boolExpr = dynamic_cast<BooleanExpr*>(expr)) {
        chunk.write(boolExpr->value ? OpCode::OP_TRUE : OpCode::OP_FALSE);
    }
    else if (auto* binary = dynamic_cast<BinaryExpr*>(expr)) {
        compileExpr(binary->left.get());
        compileExpr(binary->right.get());

        switch (binary->op.type) {
            case TokenType::PLUS:        chunk.write(OpCode::OP_ADD);   break;
            case TokenType::MINUS:       chunk.write(OpCode::OP_SUB);   break;
            case TokenType::STAR:        chunk.write(OpCode::OP_MUL);   break;
            case TokenType::SLASH:       chunk.write(OpCode::OP_DIV);   break;
            case TokenType::EQUAL_EQUAL: chunk.write(OpCode::OP_EQUAL); break;
            case TokenType::LESS_THAN:   chunk.write(OpCode::OP_LESS);  break;
            default:
                std::cerr << "Unknown binary operator: " << binary->op.lexeme << "\n";
                break;
        }
    }
    else if (auto* varExpr = dynamic_cast<VariableExpr*>(expr)) {
        uint8_t nameIdx = chunk.addName(varExpr->name.lexeme);
        chunk.write(OpCode::OP_GET_GLOBAL);
        chunk.write(nameIdx);
    }
    else if (auto* assignExpr = dynamic_cast<AssignExpr*>(expr)) {
        compileExpr(assignExpr->value.get());
        uint8_t nameIdx = chunk.addName(assignExpr->name.lexeme);
        chunk.write(OpCode::OP_SET_GLOBAL);
        chunk.write(nameIdx);
    }
    else if (auto* groupExpr = dynamic_cast<GroupingExpr*>(expr)) {
        compileExpr(groupExpr->expression.get());
    }
    else if (dynamic_cast<InputExpr*>(expr)) {
        chunk.write(OpCode::OP_INPUT);
    }
}

int Compiler::emitJump(OpCode op) {
    chunk.write(op);
    // Placeholder for 16-bit offset
    chunk.write(static_cast<uint8_t>(0xFF));
    chunk.write(static_cast<uint8_t>(0xFF));
    return static_cast<int>(chunk.code.size()) - 2; // index of the placeholder
}

void Compiler::patchJump(int offset) {
    int jump = static_cast<int>(chunk.code.size());
    chunk.code[offset]     = static_cast<uint8_t>((jump >> 8) & 0xFF);
    chunk.code[offset + 1] = static_cast<uint8_t>(jump & 0xFF);
}

void Compiler::disassemble(const Chunk& ch) {
    std::cout << "=== Bytecode ===\n";
    size_t i = 0;
    while (i < ch.code.size()) {
        std::cout << std::setw(4) << std::setfill('0') << i << "  ";

        OpCode op = static_cast<OpCode>(ch.code[i]);
        switch (op) {
            case OpCode::OP_CONST: {
                uint8_t idx = ch.code[i + 1];
                std::cout << "OP_CONST       " << ch.constants[idx] << "\n";
                i += 2; break;
            }
            case OpCode::OP_TRUE:  std::cout << "OP_TRUE\n"; i++; break;
            case OpCode::OP_FALSE: std::cout << "OP_FALSE\n"; i++; break;
            case OpCode::OP_ADD:   std::cout << "OP_ADD\n"; i++; break;
            case OpCode::OP_SUB:   std::cout << "OP_SUB\n"; i++; break;
            case OpCode::OP_MUL:   std::cout << "OP_MUL\n"; i++; break;
            case OpCode::OP_DIV:   std::cout << "OP_DIV\n"; i++; break;
            case OpCode::OP_EQUAL: std::cout << "OP_EQUAL\n"; i++; break;
            case OpCode::OP_LESS:  std::cout << "OP_LESS\n"; i++; break;
            case OpCode::OP_PRINT: std::cout << "OP_PRINT\n"; i++; break;
            case OpCode::OP_POP:   std::cout << "OP_POP\n"; i++; break;
            case OpCode::OP_INPUT: std::cout << "OP_INPUT\n"; i++; break;
            case OpCode::OP_SET_GLOBAL: {
                uint8_t idx = ch.code[i + 1];
                std::cout << "OP_SET_GLOBAL  " << ch.names[idx] << "\n";
                i += 2; break;
            }
            case OpCode::OP_GET_GLOBAL: {
                uint8_t idx = ch.code[i + 1];
                std::cout << "OP_GET_GLOBAL  " << ch.names[idx] << "\n";
                i += 2; break;
            }
            case OpCode::OP_JUMP: {
                uint16_t target = (ch.code[i + 1] << 8) | ch.code[i + 2];
                std::cout << "OP_JUMP        -> " << target << "\n";
                i += 3; break;
            }
            case OpCode::OP_JUMP_IF_FALSE: {
                uint16_t target = (ch.code[i + 1] << 8) | ch.code[i + 2];
                std::cout << "OP_JUMP_FALSE  -> " << target << "\n";
                i += 3; break;
            }
            case OpCode::OP_HALT: std::cout << "OP_HALT\n"; i++; break;
            default:
                std::cout << "UNKNOWN (" << static_cast<int>(ch.code[i]) << ")\n";
                i++; break;
        }
    }
    std::cout << "================\n";
}
