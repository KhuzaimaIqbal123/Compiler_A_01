#include <iostream>
#include <fstream>
#include <sstream>
#include "../include/lexer.hpp"    
#include "../include/token.hpp"    
#include "../include/parser.hpp"   
#include "../include/ast.hpp"      

using namespace std;

int main() {
    
    ifstream file("../sample/program.txt");
    if (!file) {
        cerr << "Error: Cannot open program.txt\n";
        return 1;
    }

    stringstream buffer;
    buffer << file.rdbuf();
    string source = buffer.str();

    // 2. Run lexer
    Lexer lexer(source);
    vector<Token> tokens = lexer.tokenize();

    cout << "=== TOKENS ===\n";
    for (auto &t : tokens) {
        cout << t.value << " (" << (int)t.type << ")\n";
    }

    // 3. Run parser
    Parser parser(tokens);
    try {
        auto program = parser.parseProgram();

        // 4. Print AST
        cout << "\n=== ABSTRACT SYNTAX TREE ===\n";
        program->print();
    } catch (const exception &e) {
        cerr << "Parser error: " << e.what() << "\n";
    }

    return 0;
}
