#include "../include/lexer.hpp"
#include "../include/lexer_no_regex.hpp"
#include <iostream>
using namespace std;

int main() {

    Lexer lexer1("sample/program.txt");
    LexerNoRegex lexer2("sample/without_regex.txt"); 


    vector<Token> l1tokens = lexer1.tokenize();
    vector<Token> l2tokens = lexer2.tokenize();

//    cout<<endl;
//    cout<<"LEXER Output with REGEX: "<<endl<<endl;    
//    for (int i = 0; i < l1tokens.size(); i++) {
//     l1tokens[i].print();
//     cout << ", ";
//     }

    cout<<endl;
    cout<<"LEXER Output without REGEX: "<<endl<<endl;
     for (int i = 0; i < l2tokens.size(); i++) {
    l2tokens[i].print();
    cout << ", ";
    }


    cout << endl;

    return 0;
}
