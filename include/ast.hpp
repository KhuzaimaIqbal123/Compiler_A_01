#ifndef AST_HPP
#define AST_HPP
#include <iostream> 
#include <string> 
#include <vector> 
#include <memory> 
using namespace std;

class ASTNode {
    public:
        virtual ~ASTNode() {}
        virtual void print(int indent = 0) const = 0;
    protected:
        void printIndent(int indent) const {
            for (int i = 0; i < indent; i++) cout << "  ";
        }
    };