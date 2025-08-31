#include "../include/lexer.hpp"
#include <iostream>
using namespace std;

int main() {

    Lexer lexer("sample/program.txt");

    vector<Token> tokens = lexer.tokenize();

   for (int i = 0; i < tokens.size(); i++) {
    tokens[i].print();
    cout << ", ";
}

    cout << endl;

    return 0;
}
