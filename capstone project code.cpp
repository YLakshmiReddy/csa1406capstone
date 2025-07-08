#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>

using namespace std;

struct Symbol {
    string var;
    int value;
    bool optimized;
};

vector<Symbol> symbolTable;

// Add or update variable
void addSymbol(string var, int value, bool optimized) {
    for (auto &s : symbolTable) {
        if (s.var == var) {
            s.value = value;
            s.optimized = optimized;
            return;
        }
    }
    symbolTable.push_back({var, value, optimized});
}

// Get symbol value if optimized
int getSymbolValue(string var, bool &found) {
    for (auto &s : symbolTable) {
        if (s.var == var && s.optimized) {
            found = true;
            return s.value;
        }
    }
    found = false;
    return 0;
}

// Trim string
string trim(const string &str) {
    size_t first = str.find_first_not_of(" \n\r\t");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \n\r\t");
    return str.substr(first, (last - first + 1));
}

// Parse and generate code
void parseLine(const string &line) {
    string var, op1, op2;
    char op;
    stringstream ss(line);
    ss >> var;
    ss.ignore(); // '='
    ss >> op1 >> op >> op2;

    bool found1 = false, found2 = false;
    int val1 = 0, val2 = 0;

    try {
        val1 = stoi(op1);
        found1 = true;
    } catch (...) {
        val1 = getSymbolValue(op1, found1);
    }

    try {
        val2 = stoi(op2);
        found2 = true;
    } catch (...) {
        val2 = getSymbolValue(op2, found2);
    }

    if (found1 && found2) {
        int result = (op == '+') ? val1 + val2 : val1 * val2;
        cout << "// Optimized: " << var << " = " << result << endl;
        addSymbol(var, result, true);
    } else {
        cout << "MOV R1, " << op1 << endl;
        cout << (op == '+' ? "ADD" : "MUL") << " R1, " << op2 << endl;
        cout << "STORE " << var << ", R1" << endl;
        addSymbol(var, 0, false);
    }
}

int main() {
    vector<string> code = {
        "x = 5 + 3;",
        "y = x * 2;",
        "z = y + 4;"
    };

    cout << "Generated Assembly Code:\n\n";
    for (const auto &line : code) {
        parseLine(trim(line));
    }

    return 0;
}
