#include <iostream>
#include <vector>
#include <string>

#include "lexer.cpp"
#include "parser.cpp"
#include "interpreter.h"
#include "ast_printer.cpp"

using namespace std;

int main() {
    cout << "=== Chikiwiki v3 Engine ===\n\n";

    // 1. Define the source program
    string source =
        "let x 20\n"
        "let y 30\n"
        "block myBlock {\n"
        "  let z x + y * 2\n"
        "  print z\n"
        "}\n"
        "run myBlock\n"
        "let a (x + y) * 2\n"
        "print a\n";

    cout << "--- Source Code ---\n" << source << "\n";

    // 2. Lexical Analysis (Tokenization)
    cout << "--- Lexing... ---\n";
    Lexer lexer(source);
    vector<Token> tokens = lexer.tokenize();
    
    cout << "Tokens generated:\n";
    for (const auto& token : tokens) {
        cout << "  [" << token.type << ": \"" << token.value << "\"]\n";
    }
    cout << "\n";

    // 3. Parsing
    cout << "--- Parsing AST... ---\n";
    Parser parser(tokens);
    vector<Stmt*> program = parser.parseProgram();
    cout << "AST built successfully!\n\n";

    // 4. Print AST
    cout << "--- AST Structure ---\n";
    for (auto stmt : program) {
        printStmt(stmt, 0);
    }
    cout << "\n";

    // 5. Execution
    cout << "--- Executing Program... ---\n";
    Interpreter interp;
    interp.executeProgram(program);
    cout << "\n=== Execution Finished Successfully ===\n";

    // 6. Cleanup Memory
    for (auto stmt : program) {
        delete stmt;
    }

    return 0;
}