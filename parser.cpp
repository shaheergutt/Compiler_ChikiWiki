#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include "lexer.h"
#include "ast.h"

using namespace std;

class Parser {
private:
    vector<Token> tokens;
    size_t pos = 0;

    Token peek() {
        if (pos >= tokens.size()) {
            return {END_OF_FILE, "EOF"};
        }
        return tokens[pos];
    }

    Token previous() {
        if (pos == 0) return {END_OF_FILE, "EOF"};
        return tokens[pos - 1];
    }

    Token get() {
        if (pos >= tokens.size()) {
            return {END_OF_FILE, "EOF"};
        }
        return tokens[pos++];
    }

    bool isAtEnd() {
        return peek().type == END_OF_FILE;
    }

    bool match(TokenType type) {
        if (peek().type == type) {
            get();
            return true;
        }
        return false;
    }

    Token consume(TokenType type, const string& message) {
        if (peek().type == type) {
            return get();
        }
        throw runtime_error("Parser Error: " + message + " Got: " + peek().value);
    }

public:
    Parser(const vector<Token>& t) : tokens(t) {}

    Expr* parsePrimary() {
        if (match(NUMBER)) {
            return new NumberExpr(stoi(previous().value));
        }

        if (match(IDENTIFIER)) {
            return new VariableExpr(previous().value);
        }

        if (match(LPAREN)) {
            Expr* expr = parseExpression();
            consume(RPAREN, "Expect ')' after expression.");
            return expr;
        }

        throw runtime_error("Expect expression, got: '" + peek().value + "'");
    }

    Expr* parseTerm() {
        Expr* left = parsePrimary();

        while (match(STAR) || match(SLASH)) {
            TokenType opType = previous().type;
            string opStr = (opType == STAR) ? "*" : "/";
            Expr* right = parsePrimary();
            left = new BinaryExpr(left, opStr, right);
        }

        return left;
    }

    Expr* parseExpression() {
        Expr* left = parseTerm();

        while (match(PLUS) || match(MINUS)) {
            TokenType opType = previous().type;
            string opStr = (opType == PLUS) ? "+" : "-";
            Expr* right = parseTerm();
            left = new BinaryExpr(left, opStr, right);
        }

        return left;
    }

    Stmt* parseStatement() {
        try {
            if (match(LET)) {
                consume(IDENTIFIER, "Expect variable name after 'let'.");
                string name = previous().value;
                Expr* val = parseExpression();
                return new LetStmt(name, val);
            }

            if (match(PRINT)) {
                Expr* val = parseExpression();
                return new PrintStmt(val);
            }

            if (match(RUN)) {
                consume(IDENTIFIER, "Expect block name after 'run'.");
                return new RunStmt(previous().value);
            }

            if (match(BLOCK)) {
                consume(IDENTIFIER, "Expect block name after 'block'.");
                string name = previous().value;
                consume(LBRACE, "Expect '{' to start block.");
                vector<Stmt*> statements;
                while (peek().type != RBRACE && peek().type != END_OF_FILE) {
                    Stmt* stmt = parseStatement();
                    if (stmt) {
                        statements.push_back(stmt);
                    }
                }
                consume(RBRACE, "Expect '}' to end block.");
                return new BlockStmt(name, statements);
            }

            if (!isAtEnd()) {
                Token t = get();
                throw runtime_error("Unexpected token '" + t.value + "'");
            }
        } catch (const runtime_error& err) {
            cerr << err.what() << endl;
            sync();
        }
        return nullptr;
    }

    void sync() {
        while (!isAtEnd()) {
            switch (peek().type) {
                case LET:
                case PRINT:
                case RUN:
                case BLOCK:
                case RBRACE:
                    return;
                default:
                    get();
            }
        }
    }

    vector<Stmt*> parseProgram() {
        vector<Stmt*> program;
        while (!isAtEnd()) {
            Stmt* stmt = parseStatement();
            if (stmt) {
                program.push_back(stmt);
            }
        }
        return program;
    }
};