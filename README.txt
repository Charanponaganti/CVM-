================================================================================
                         CVM++ — Compiler & Virtual Machine
================================================================================

  A custom programming language with a complete pipeline:
  Source Code (.cvm) → Lexer → Parser → Compiler → Bytecode → Virtual Machine


================================================================================
  TABLE OF CONTENTS
================================================================================

  1. Project Overview
  2. Building from Source
  3. Running Programs
  4. Using the REPL
  5. Debug Mode
  6. Language Reference
  7. Sample Programs
  8. Architecture Overview
  9. Bytecode Instruction Set


================================================================================
  1. PROJECT OVERVIEW
================================================================================

  CVM++ is a stack-based virtual machine and bytecode compiler for a simple
  imperative language. Source files use the ".cvm" extension and are compiled
  to bytecode that runs on the built-in VM — all in a single executable.

  The pipeline works as follows:

    ┌──────────┐    ┌────────┐    ┌──────────┐    ┌────────┐
    │  Source   │ →  │  Lexer │ →  │  Parser  │ →  │Compiler│ →  Bytecode
    │ (.cvm)   │    │(Tokens)│    │  (AST)   │    │(Chunk) │
    └──────────┘    └────────┘    └──────────┘    └────────┘
                                                       │
                                                       ▼
                                                  ┌─────────┐
                                                  │   VM    │ →  Output
                                                  │(Execute)│
                                                  └─────────┘


================================================================================
  2. BUILDING FROM SOURCE
================================================================================

  Prerequisites:
    • A C++17 (or later) compiler (g++, clang++, or MSVC)

  ── Using g++ (MinGW / Linux / macOS) ──────────────────────────────────────

    Open a terminal in the project root directory and run:

      g++ -std=c++17 -o main.exe ^
          src/main.cpp ^
          src/lexer/lexer.cpp ^
          src/parser/parser.cpp ^
          src/compiler/compiler.cpp ^
          src/vm/vm.cpp

    (On Linux / macOS, replace "main.exe" with "main" and use "\" instead
     of "^" for line continuation.)

  ── Using MSVC (Visual Studio Developer Prompt) ───────────────────────────

      cl /EHsc /std:c++17 ^
          src/main.cpp ^
          src/lexer/lexer.cpp ^
          src/parser/parser.cpp ^
          src/compiler/compiler.cpp ^
          src/vm/vm.cpp ^
          /Fe:main.exe


================================================================================
  3. RUNNING PROGRAMS
================================================================================

  To run a .cvm source file:

      main.exe <filename.cvm>

  Examples:

      main.exe examples/sum.cvm
      main.exe examples/calculator.cvm
      main.exe examples/truth_machine.cvm

  The compiler reads the source file, compiles it to bytecode internally,
  and immediately executes the bytecode on the virtual machine.


================================================================================
  4. USING THE REPL
================================================================================

  Launch the interactive REPL by running the executable with no arguments:

      main.exe

  You will see:

      CVM++ REPL v1.0
      Type 'exit' to quit, ':debug' to toggle debug mode.

      cvm>

  Type any valid CVM++ expression or statement and press Enter:

      cvm> print 10 + 20
      30
      cvm> let x = 42
      cvm> print x * 2
      84
      cvm> exit
      Goodbye!


================================================================================
  5. DEBUG MODE
================================================================================

  Debug mode shows the full compilation pipeline (tokens, AST, and bytecode)
  before executing the program. This is useful for understanding how your
  code is compiled.

  ── From the command line ──────────────────────────────────────────────────

      main.exe examples/sum.cvm --debug

  ── From the REPL ─────────────────────────────────────────────────────────

      cvm> :debug
      Debug mode ON
      cvm> print 3 + 4

      --- Tokens ---
        print
        3
        +
        4
        
      --- AST ---
      PrintStmt
        Binary(+)
          Number(3)
          Number(4)

      === Bytecode ===
      0000  OP_CONST       3
      0002  OP_CONST       4
      0004  OP_ADD
      0005  OP_PRINT
      0006  OP_HALT
      ================

      --- Output ---
      7


================================================================================
  6. LANGUAGE REFERENCE
================================================================================

  ── Data Types ─────────────────────────────────────────────────────────────

    • Numbers     64-bit floating point (e.g., 42, 100, 0)
    • Booleans    true, false (internally 1.0 and 0.0)

  ── Variables ──────────────────────────────────────────────────────────────

    Declare with 'let':

      let x = 10
      let name = 0

    Reassign (no 'let' keyword):

      x = x + 1

  ── Arithmetic Operators ───────────────────────────────────────────────────

      +    Addition
      -    Subtraction
      *    Multiplication
      /    Division

  ── Comparison Operators ───────────────────────────────────────────────────

      <    Less than
      ==   Equal to

  ── Grouping ───────────────────────────────────────────────────────────────

    Use parentheses to control evaluation order:

      let result = (2 + 3) * 4

  ── Print Statement ────────────────────────────────────────────────────────

      print <expression>

    Prints the value of the expression followed by a newline.
    Integer values are printed without decimal points.

  ── Input Expression ───────────────────────────────────────────────────────

      input

    Reads a number from standard input (shows ">>> " prompt).
    Can be used anywhere an expression is expected:

      let x = input
      print input + 10

  ── If / Else ──────────────────────────────────────────────────────────────

      if <condition> {
          <statements>
      }

      if <condition> {
          <statements>
      } else {
          <statements>
      }

    The condition is considered false if it equals 0; any other value
    is considered true.

  ── While Loop ─────────────────────────────────────────────────────────────

      while <condition> {
          <statements>
      }

    Repeats the body as long as the condition is non-zero (true).


================================================================================
  7. SAMPLE PROGRAMS
================================================================================

  All sample programs are in the "examples/" folder.

  ── examples/calculator.cvm ────────────────────────────────────────────────

    An interactive calculator. The user picks an operation (1–4), enters
    two numbers, and sees the result. Runs in a loop until the user
    chooses option 0 to quit.

    Run:   main.exe examples/calculator.cvm

    Sample session:

        ========================
        Simple Calculator
        ========================
        1 = Add
        2 = Subtract
        3 = Multiply
        4 = Divide
        0 = Quit
        ========================
        Choose operation:
        >>> 1
        Enter first number:
        >>> 10
        Enter second number:
        >>> 25
        Result:
        35
        ...

  ── examples/truth_machine.cvm ─────────────────────────────────────────────

    A classic truth machine:
      • Enter 0  →  prints 0 once, then stops.
      • Enter 1  →  prints 1 forever (infinite loop).

    Run:   main.exe examples/truth_machine.cvm

    Sample session (input 0):

        Truth Machine
        Enter 0 or 1:
        >>> 0
        0

    Sample session (input 1):

        Truth Machine
        Enter 0 or 1:
        >>> 1
        1
        1
        1
        ... (press Ctrl+C to stop)

  ── examples/sum.cvm ───────────────────────────────────────────────────────

    Calculates the sum of numbers from 1 to 10 using a while loop.

    Run:   main.exe examples/sum.cvm

    Output:

        55

  ── examples/demo.cvm ──────────────────────────────────────────────────────

    Demonstrates arithmetic, if/else branching, and while loops.

    Run:   main.exe examples/demo.cvm

    Output:

        15
        30
        1
        0
        1
        2
        3
        4
        99


================================================================================
  8. ARCHITECTURE OVERVIEW
================================================================================

  Project structure:

    cvm/
    ├── main.exe                  Compiled executable
    ├── README.txt                This file
    ├── examples/
    │   ├── sum.cvm               Sum 1..10
    │   ├── demo.cvm              Feature demo
    │   ├── calculator.cvm        Interactive calculator
    │   └── truth_machine.cvm     Truth machine
    └── src/
        ├── main.cpp              Entry point, REPL, file runner
        ├── lexer/
        │   ├── Token.h           Token types & struct
        │   ├── lexer.h           Lexer class declaration
        │   └── lexer.cpp         Lexer implementation (scanner)
        ├── parser/
        │   ├── AST.h             Expression AST node types
        │   ├── Stmt.h            Statement AST node types
        │   ├── parser.h          Parser class declaration
        │   └── parser.cpp        Recursive descent parser
        ├── compiler/
        │   ├── opcode.h          Opcodes & Chunk (bytecode container)
        │   ├── compiler.h        Compiler class declaration
        │   └── compiler.cpp      AST → bytecode compiler
        └── vm/
            ├── vm.h              VM class declaration
            └── vm.cpp            Stack-based bytecode interpreter

  Pipeline stages:

    1. LEXER      Scans source text into a stream of tokens.
    2. PARSER     Builds an Abstract Syntax Tree (AST) from tokens
                  using recursive descent parsing.
    3. COMPILER   Walks the AST and emits bytecode into a Chunk,
                  with jump-patching for if/else and while.
    4. VM         Executes bytecode using a 256-slot value stack
                  and a global variable table.


================================================================================
  9. BYTECODE INSTRUCTION SET
================================================================================

  Opcode              Operand         Description
  ─────────────────── ─────────────── ──────────────────────────────────────
  OP_CONST            index (1 byte)  Push constants[index] onto stack
  OP_TRUE             —               Push 1.0 (true)
  OP_FALSE            —               Push 0.0 (false)
  OP_ADD              —               Pop a, b; push a + b
  OP_SUB              —               Pop a, b; push a - b
  OP_MUL              —               Pop a, b; push a * b
  OP_DIV              —               Pop a, b; push a / b
  OP_EQUAL            —               Pop a, b; push (a == b)
  OP_LESS             —               Pop a, b; push (a < b)
  OP_PRINT            —               Pop and print top of stack
  OP_POP              —               Discard top of stack
  OP_INPUT            —               Read number from stdin, push
  OP_SET_GLOBAL       index (1 byte)  Set global variable names[index]
  OP_GET_GLOBAL       index (1 byte)  Get global variable names[index]
  OP_JUMP             target (2 bytes) Unconditional jump to target
  OP_JUMP_IF_FALSE    target (2 bytes) Pop; jump to target if value == 0
  OP_HALT             —               Stop execution

  Use --debug mode to see the disassembled bytecode for any program.

================================================================================
