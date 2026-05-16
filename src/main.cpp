#include "lexer/lexer.h"
#include "parser/parser.h"
#include "compiler/compiler.h"
#include "vm/vm.h"
#include <iostream>
#include <fstream>
#include <sstream>

// Run source code through the full pipeline: Lexer -> Parser -> Compiler -> VM
void run(const std::string& source, bool debug = false) {
    // 1. Lexing
    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    if (debug) {
        std::cout << "\n--- Tokens ---\n";
        for (const auto& t : tokens) {
            std::cout << "  " << t.lexeme << "\n";
        }
    }

    // 2. Parsing
    Parser parser(tokens);
    auto statements = parser.parse();

    if (debug) {
        std::cout << "\n--- AST ---\n";
        for (const auto& stmt : statements) {
            if (stmt) stmt->dump();
        }
    }

    // 3. Compiling
    Compiler compiler;
    Chunk chunk = compiler.compile(statements);

    if (debug) {
        std::cout << "\n";
        compiler.disassemble(chunk);
        std::cout << "\n--- Output ---\n";
    }

    // 4. Executing
    VM vm;
    VM::Result result = vm.execute(chunk);

    if (result == VM::Result::RUNTIME_ERROR) {
        std::cerr << "[VM] Execution failed with runtime error.\n";
    }
}

// Read entire file into a string
std::string readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file '" << path << "'\n";
        return "";
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

void repl() {
    std::cout << "CVM++ REPL v1.0\n";
    std::cout << "Type 'exit' to quit, ':debug' to toggle debug mode.\n\n";

    bool debug = false;
    std::string line;

    while (true) {
        std::cout << "cvm> ";
        if (!std::getline(std::cin, line)) break;

        if (line == "exit") break;
        if (line == ":debug") {
            debug = !debug;
            std::cout << "Debug mode " << (debug ? "ON" : "OFF") << "\n";
            continue;
        }
        if (line.empty()) continue;

        run(line, debug);
    }

    std::cout << "Goodbye!\n";
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        // No arguments -> start REPL
        repl();
    }
    else if (argc >= 2) {
        std::string filename = argv[1];
        bool debug = false;

        // Check for --debug flag
        for (int i = 2; i < argc; i++) {
            if (std::string(argv[i]) == "--debug") {
                debug = true;
            }
        }

        std::string source = readFile(filename);
        if (!source.empty()) {
            run(source, debug);
        }
    }

    return 0;
}