#include "../include/lexer_no_regex.hpp"
#include <iostream>
using namespace std;

int main() {

    LexerNoRegex lexer2("../sample/without_regex.txt"); 


    vector<Token> l2tokens = lexer2.tokenize();

    cout<<endl;
    cout<<"LEXER Output without REGEX: "<<endl<<endl;
     for (int i = 0; i < l2tokens.size(); i++) {
    l2tokens[i].print();
    cout << ", ";
    }

    cout << endl;

    return 0;
}
