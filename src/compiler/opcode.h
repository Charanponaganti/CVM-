#pragma once

#include <cstdint>
#include <string>
#include <vector>

enum class OpCode : uint8_t {
    OP_CONST,           // Push a constant onto the stack
    OP_TRUE,            // Push true
    OP_FALSE,           // Push false

    OP_ADD,             // Pop two, push sum
    OP_SUB,             // Pop two, push difference
    OP_MUL,             // Pop two, push product
    OP_DIV,             // Pop two, push quotient

    OP_EQUAL,           // Pop two, push (a == b)
    OP_LESS,            // Pop two, push (a < b)

    OP_PRINT,           // Pop and print top of stack
    OP_POP,             // Discard top of stack
    OP_INPUT,           // Read number from stdin, push onto stack

    OP_SET_GLOBAL,      // Set a global variable (operand = name index)
    OP_GET_GLOBAL,      // Get a global variable (operand = name index)

    OP_JUMP,            // Unconditional jump (operand = offset)
    OP_JUMP_IF_FALSE,   // Jump if top of stack is falsy (operand = offset)

    OP_HALT             // Stop execution
};

struct Chunk {
    std::vector<uint8_t> code;          // The bytecode stream
    std::vector<double> constants;      // Constant pool (numbers)
    std::vector<std::string> names;     // Variable name pool

    // Emit a single byte
    void write(uint8_t byte) {
        code.push_back(byte);
    }

    // Emit an opcode
    void write(OpCode op) {
        code.push_back(static_cast<uint8_t>(op));
    }

    // Add a constant, return its index
    uint8_t addConstant(double value) {
        constants.push_back(value);
        return static_cast<uint8_t>(constants.size() - 1);
    }

    // Add a variable name, return its index (reuse if exists)
    uint8_t addName(const std::string& name) {
        for (size_t i = 0; i < names.size(); i++) {
            if (names[i] == name) return static_cast<uint8_t>(i);
        }
        names.push_back(name);
        return static_cast<uint8_t>(names.size() - 1);
    }
};
