#pragma once
#include <iostream>
#include "ast.h"

using namespace std;

void printExpr(Expr* expr, int depth);

void printStmt(Stmt* stmt, int depth = 0) {
    if (!stmt) return;

    string indent(depth * 2, ' ');

    if (auto let = dynamic_cast<LetStmt*>(stmt)) {
        cout << indent << "LetStmt(" << let->name << ")\n";
        printExpr(let->value, depth + 1);
    }
    else if (auto prt = dynamic_cast<PrintStmt*>(stmt)) {
        cout << indent << "PrintStmt\n";
        printExpr(prt->value, depth + 1);
    }
    else if (auto run = dynamic_cast<RunStmt*>(stmt)) {
        cout << indent << "RunStmt(" << run->blockName << ")\n";
    }
    else if (auto block = dynamic_cast<BlockStmt*>(stmt)) {
        cout << indent << "BlockStmt(" << block->name << ") {\n";
        for (auto s : block->statements) {
            printStmt(s, depth + 1);
        }
        cout << indent << "}\n";
    }
}

void printExpr(Expr* expr, int depth = 0) {
    if (!expr) return;

    string indent(depth * 2, ' ');

    if (auto num = dynamic_cast<NumberExpr*>(expr)) {
        cout << indent << "Number(" << num->value << ")\n";
    }
    else if (auto var = dynamic_cast<VariableExpr*>(expr)) {
        cout << indent << "Variable(" << var->name << ")\n";
    }
    else if (auto bin = dynamic_cast<BinaryExpr*>(expr)) {
        cout << indent << "Binary(" << bin->op << ")\n";
        printExpr(bin->left, depth + 1);
        printExpr(bin->right, depth + 1);
    }
}