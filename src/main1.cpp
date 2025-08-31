#include "../include/lexer.hpp"
#include <iostream>
using namespace std;

int main() {

    Lexer lexer1("../sample/program.txt");


    vector<Token> l1tokens = lexer1.tokenize();

   cout<<endl;
   cout<<"LEXER with REGEX: "<<endl<<endl;    
   for (int i = 0; i < l1tokens.size(); i++) {
    l1tokens[i].print();
    cout << ", ";
    }

    cout << endl;

    return 0;
}
