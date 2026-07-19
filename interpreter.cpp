#include "interpreter.h"
#include <iostream>

using namespace std;

int Interpreter::eval(Expr* expr) {
    if (!expr) return 0;

    if (auto num = dynamic_cast<NumberExpr*>(expr)) {
        return num->value;
    }

    if (auto var = dynamic_cast<VariableExpr*>(expr)) {
        // Return 0 if variable doesn't exist yet, to avoid creating it with default mapping
        if (vars.count(var->name)) {
            return vars[var->name];
        }
        return 0;
    }

    if (auto bin = dynamic_cast<BinaryExpr*>(expr)) {
        int left = eval(bin->left);
        int right = eval(bin->right);

        if (bin->op == "+") return left + right;
        if (bin->op == "-") return left - right;
        if (bin->op == "*") return left * right;
        if (bin->op == "/") return right != 0 ? left / right : 0;
    }

    return 0;
}

void Interpreter::execute(Stmt* stmt) {
    if (!stmt) return;

    if (auto let = dynamic_cast<LetStmt*>(stmt)) {
        vars[let->name] = eval(let->value);
        return;
    }

    if (auto prt = dynamic_cast<PrintStmt*>(stmt)) {
        cout << eval(prt->value) << endl;
        return;
    }

    if (auto run = dynamic_cast<RunStmt*>(stmt)) {
        if (blocks.count(run->blockName)) {
            for (auto s : blocks[run->blockName]->statements) {
                execute(s);
            }
        } else {
            cout << "Block not found: " << run->blockName << endl;
        }
        return;
    }

    if (auto block = dynamic_cast<BlockStmt*>(stmt)) {
        // Register the block
        blocks[block->name] = block;
        return;
    }
}

void Interpreter::executeProgram(const vector<Stmt*>& program) {
    for (auto stmt : program) {
        execute(stmt);
    }
}