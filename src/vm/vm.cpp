#include "vm.h"
#include <iostream>
#include <string>

VM::Result VM::execute(const Chunk& ch) {
    chunk = &ch;
    ip = 0;
    sp = 0;

    while (ip < chunk->code.size()) {
        OpCode op = readOp();

        switch (op) {
            case OpCode::OP_CONST: {
                uint8_t idx = readByte();
                push(chunk->constants[idx]);
                break;
            }

            case OpCode::OP_TRUE:  push(1.0); break;
            case OpCode::OP_FALSE: push(0.0); break;

            case OpCode::OP_ADD: {
                double b = pop();
                double a = pop();
                push(a + b);
                break;
            }
            case OpCode::OP_SUB: {
                double b = pop();
                double a = pop();
                push(a - b);
                break;
            }
            case OpCode::OP_MUL: {
                double b = pop();
                double a = pop();
                push(a * b);
                break;
            }
            case OpCode::OP_DIV: {
                double b = pop();
                double a = pop();
                if (b == 0) {
                    std::cerr << "Runtime Error: Division by zero.\n";
                    return Result::RUNTIME_ERROR;
                }
                push(a / b);
                break;
            }

            case OpCode::OP_EQUAL: {
                double b = pop();
                double a = pop();
                push(a == b ? 1.0 : 0.0);
                break;
            }
            case OpCode::OP_LESS: {
                double b = pop();
                double a = pop();
                push(a < b ? 1.0 : 0.0);
                break;
            }

            case OpCode::OP_PRINT: {
                double val = pop();
                // Print integers cleanly, doubles with decimals
                if (val == static_cast<int>(val)) {
                    std::cout << static_cast<int>(val) << std::endl;
                } else {
                    std::cout << val << std::endl;
                }
                break;
            }

            case OpCode::OP_POP: {
                pop();
                break;
            }

            case OpCode::OP_INPUT: {
                std::string line;
                std::cout << ">>> ";
                std::getline(std::cin, line);
                try {
                    double val = std::stod(line);
                    push(val);
                } catch (...) {
                    std::cerr << "Runtime Error: Invalid number input.\n";
                    return Result::RUNTIME_ERROR;
                }
                break;
            }

            case OpCode::OP_SET_GLOBAL: {
                uint8_t idx = readByte();
                const std::string& name = chunk->names[idx];
                double val = peek(); // keep value on stack
                globals[name] = val;
                break;
            }

            case OpCode::OP_GET_GLOBAL: {
                uint8_t idx = readByte();
                const std::string& name = chunk->names[idx];
                auto it = globals.find(name);
                if (it == globals.end()) {
                    std::cerr << "Runtime Error: Undefined variable '" << name << "'.\n";
                    return Result::RUNTIME_ERROR;
                }
                push(it->second);
                break;
            }

            case OpCode::OP_JUMP: {
                uint8_t hi = readByte();
                uint8_t lo = readByte();
                uint16_t target = (hi << 8) | lo;
                ip = target;
                break;
            }

            case OpCode::OP_JUMP_IF_FALSE: {
                uint8_t hi = readByte();
                uint8_t lo = readByte();
                uint16_t target = (hi << 8) | lo;
                double cond = pop();
                if (cond == 0.0) {
                    ip = target;
                }
                break;
            }

            case OpCode::OP_HALT:
                return Result::OK;

            default:
                std::cerr << "Runtime Error: Unknown opcode " << static_cast<int>(op) << "\n";
                return Result::RUNTIME_ERROR;
        }
    }

    return Result::OK;
}

uint8_t VM::readByte() {
    return chunk->code[ip++];
}

OpCode VM::readOp() {
    return static_cast<OpCode>(readByte());
}

void VM::push(double value) {
    if (sp >= STACK_MAX) {
        std::cerr << "Runtime Error: Stack overflow.\n";
        return;
    }
    stack[sp++] = value;
}

double VM::pop() {
    if (sp <= 0) {
        std::cerr << "Runtime Error: Stack underflow.\n";
        return 0;
    }
    return stack[--sp];
}

double VM::peek() {
    if (sp <= 0) return 0;
    return stack[sp - 1];
}
