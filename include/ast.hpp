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

class Expr : public ASTNode {};

class LiteralExpr : public Expr {
    string value;
    public:
        LiteralExpr(const string& val) : value(val) {}
        void print(int indent = 0) const override {
            printIndent(indent);
            cout << "Literal: " << value << "\n";
        }
    };

class IdentifierExpr : public Expr {
    string name;
    public:
        IdentifierExpr(const string& n) : name(n) {}
        void print(int indent = 0) const override {
            printIndent(indent);
            cout << "Identifier: " << name << "\n";
        }
    };
    

    