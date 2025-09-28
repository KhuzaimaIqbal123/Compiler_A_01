#include "../include/lexer.hpp"
#include "../include/parser.hpp"
#include "../include/ast.hpp"
#include <iostream>
using namespace std;

int main() {
    
    Lexer lexer("sample/program.txt");


    
    vector<Token> tokens = lexer.tokenize();

    for (auto &t : tokens) {
        t.print();
        cout << ", ";
    }
    cout << "\nTotal Tokens: " << tokens.size() << "\n\n";

    Parser parser(tokens);
    auto program = parser.parseProgram();

    cout << "=== ABSTRACT SYNTAX TREE ===\n";
    program->print(0);
}
