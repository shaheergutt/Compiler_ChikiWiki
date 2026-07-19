#pragma once
#include <map>
#include <string>
#include <vector>
#include "ast.h"

using namespace std;

class Interpreter {
public:
    map<string, int> vars;
    map<string, BlockStmt*> blocks;

    int eval(Expr* expr);
    void execute(Stmt* stmt);
    void executeProgram(const vector<Stmt*>& program);
};