#pragma once
#include <string>
#include <vector>
using namespace std;

enum TokenType {
    LET,
    PRINT,
    RUN,
    BLOCK,
    END,
    IDENTIFIER,
    NUMBER,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    LBRACE,
    RBRACE,
    LPAREN,
    RPAREN,
    END_OF_FILE
};

struct Token {
    TokenType type;
    string value;
};