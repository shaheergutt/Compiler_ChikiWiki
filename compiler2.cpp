#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <string>
#include <cctype>

using namespace std;

map<string, int> vars;
map<string, vector<string>> blocks;

string clean(string s) {
    string r;
    for (char c : s)
        if (c != ' ' && c != '\t')
            r += c;
    return r;
}

int getValue(string token) {
    if (token.empty()) return 0;

    if (isdigit(token[0]) || token[0] == '-') return stoi(token);

    if (vars.count(token)) return vars[token];

    return 0;
}

int evaluate(string expr) {
    expr = clean(expr);

    char op = 0;
    int pos = -1;

    for (int i = 0; i < expr.size(); i++) {
        if (expr[i] == '+' || expr[i] == '-' ||
            expr[i] == '*' || expr[i] == '/') {
            op = expr[i];
            pos = i;
            break;
        }
    }

    if (!op) return getValue(expr);

    string left = expr.substr(0, pos);
    string right = expr.substr(pos + 1);

    int a = getValue(left);
    int b = getValue(right);

    if (op == '+') return a + b;
    if (op == '-') return a - b;
    if (op == '*') return a * b;
    if (op == '/') return (b ? a / b : 0);

    return 0;
}

void execute(string line);

void runBlock(string name) {
    if (!blocks.count(name)) {
        cout << "Block not found: " << name << endl;
        return;
    }

    for (string line : blocks[name]) {
        execute(line);
    }
}

void execute(string line) {
    stringstream ss(line);
    string cmd;
    ss >> cmd;

    if (cmd == "let") {
        string name, expr;
        ss >> name;
        getline(ss, expr);
        if (!expr.empty()) expr = expr.substr(1);

        vars[name] = evaluate(expr);
    }

    else if (cmd == "print") {
        string expr;
        getline(ss, expr);
        if (!expr.empty()) expr = expr.substr(1);

        cout << evaluate(expr) << endl;
    }

    else if (cmd == "run") {
        string name;
        ss >> name;
        runBlock(name);
    }
}

int main() {
    cout << "Chikiwiki FINAL SAFE ENGINE\n";
    cout << "Commands: run | exit\n\n";

    string line;
    string currentBlock = "";
    bool inBlock = false;

    while (true) {
        cout << ">>> ";
        getline(cin, line);

        if (line == "exit") break;

        string trimmed = line;
        trimmed.erase(0, trimmed.find_first_not_of(" \t"));

        // BLOCK START
        if (trimmed.find("block") == 0 && trimmed.find("{") != string::npos) {
            string name = trimmed.substr(6);
            name = name.substr(0, name.find("{"));
            name.erase(0, name.find_first_not_of(" \t"));

            currentBlock = name;
            inBlock = true;
            blocks[currentBlock] = {};
            continue;
        }

        // BLOCK END
        if (trimmed == "}" || trimmed == "end") {
            inBlock = false;
            currentBlock = "";
            continue;
        }

        // STORE INSIDE BLOCK
        if (inBlock) {
            blocks[currentBlock].push_back(trimmed);
        }
        else {
            execute(trimmed);
        }
    }

    return 0;
}