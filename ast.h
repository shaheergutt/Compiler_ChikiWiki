#pragma once
#include <string>
#include <vector>

using namespace std;

// Base class for all Expression nodes
class Expr {
public:
    virtual ~Expr() = default;
};

class NumberExpr : public Expr {
public:
    int value;
    NumberExpr(int v) : value(v) {}
};

class VariableExpr : public Expr {
public:
    string name;
    VariableExpr(string n) : name(n) {}
};

class BinaryExpr : public Expr {
public:
    Expr* left;
    string op;
    Expr* right;

    BinaryExpr(Expr* l, string o, Expr* r)
        : left(l), op(o), right(r) {}

    virtual ~BinaryExpr() override {
        delete left;
        delete right;
    }
};

// Base class for all Statement nodes
class Stmt {
public:
    virtual ~Stmt() = default;
};

class LetStmt : public Stmt {
public:
    string name;
    Expr* value;

    LetStmt(string n, Expr* v) : name(n), value(v) {}
    virtual ~LetStmt() override {
        delete value;
    }
};

class PrintStmt : public Stmt {
public:
    Expr* value;

    PrintStmt(Expr* v) : value(v) {}
    virtual ~PrintStmt() override {
        delete value;
    }
};

class RunStmt : public Stmt {
public:
    string blockName;

    RunStmt(string name) : blockName(name) {}
};

class BlockStmt : public Stmt {
public:
    string name;
    vector<Stmt*> statements;

    BlockStmt(string n, vector<Stmt*> stmts) : name(n), statements(stmts) {}
    virtual ~BlockStmt() override {
        for (auto stmt : statements) {
            delete stmt;
        }
    }
};