#pragma once

#include "../compiler/opcode.h"
#include <vector>
#include <unordered_map>
#include <string>

class VM {
private:
    static const int STACK_MAX = 256;
    double stack[STACK_MAX];
    int sp = 0; // stack pointer

    std::unordered_map<std::string, double> globals; // global variables

    const Chunk* chunk;
    size_t ip; // instruction pointer

public:
    enum class Result {
        OK,
        COMPILE_ERROR,
        RUNTIME_ERROR
    };

    Result execute(const Chunk& chunk);

private:
    uint8_t readByte();
    OpCode readOp();
    double pop();
    void push(double value);
    double peek();
};
