#include "lexer.h"
#include <cctype>

class Lexer {
public:
    string source;
    size_t pos = 0;

    Lexer(string s) {
        source = s;
    }

    vector<Token> tokenize() {
        vector<Token> tokens;

        while (pos < source.size()) {
            char c = source[pos];

            if (isspace(c)) {
                pos++;
                continue;
            }

            // identifier / keyword
            if (isalpha(c)) {
                string word;
                while (pos < source.size() && isalnum(source[pos])) {
                    word += source[pos++];
                }

                if (word == "let")
                    tokens.push_back({LET, word});
                else if (word == "print")
                    tokens.push_back({PRINT, word});
                else if (word == "run")
                    tokens.push_back({RUN, word});
                else if (word == "block")
                    tokens.push_back({BLOCK, word});
                else if (word == "end")
                    tokens.push_back({END, word});
                else
                    tokens.push_back({IDENTIFIER, word});

                continue;
            }

            // number
            if (isdigit(c)) {
                string num;
                while (pos < source.size() && isdigit(source[pos])) {
                    num += source[pos++];
                }

                tokens.push_back({NUMBER, num});
                continue;
            }

            switch (c) {
                case '+': tokens.push_back({PLUS, "+"}); break;
                case '-': tokens.push_back({MINUS, "-"}); break;
                case '*': tokens.push_back({STAR, "*"}); break;
                case '/': tokens.push_back({SLASH, "/"}); break;
                case '{': tokens.push_back({LBRACE, "{"}); break;
                case '}': tokens.push_back({RBRACE, "}"}); break;
                case '(': tokens.push_back({LPAREN, "("}); break;
                case ')': tokens.push_back({RPAREN, ")"}); break;
            }

            pos++;
        }

        tokens.push_back({END_OF_FILE, "EOF"});
        return tokens;
    }
};