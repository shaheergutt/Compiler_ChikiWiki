#include <iostream>
#include "parser.cpp"

using namespace std;

int main() {

    vector<Token> tokens = {
        {IDENT, "x"},
        {PLUS, "+"},
        {NUMBER, "5"},
        {END, ""}
    };

    Parser parser(tokens);

    Expr* ast = parser.parseExpression();

    cout << "Parser built AST successfully\n";

    return 0;
}