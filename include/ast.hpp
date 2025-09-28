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
    
class BinaryExpr : public Expr {
    string op;
    unique_ptr<Expr> left, right;
    public:
        BinaryExpr(const string& oper, unique_ptr<Expr> l, unique_ptr<Expr> r)
            : op(oper), left(move(l)), right(move(r)) {}
        void print(int indent = 0) const override {
            printIndent(indent);
            cout << "BinaryExpr: " << op << "\n";
            left->print(indent + 1);
            right->print(indent + 1);
        }
};

class Stmt : public ASTNode {};

class VarDecl : public Stmt {
    string type, name;
    unique_ptr<Expr> init;
public:
    VarDecl(const string& t, const string& n, unique_ptr<Expr> i = nullptr)
        : type(t), name(n), init(move(i)) {}
    void print(int indent = 0) const override {
        printIndent(indent);
        cout << "VarDecl: " << type << " " << name << "\n";
        if (init) init->print(indent + 1);
    }
};


class ReturnStmt : public Stmt {
    unique_ptr<Expr> value;
public:
    ReturnStmt(unique_ptr<Expr> v) : value(move(v)) {}
    void print(int indent = 0) const override {
        printIndent(indent);
        cout << "ReturnStmt\n";
        if (value) value->print(indent + 1);
    }
};

class FunctionDecl : public Stmt {
    string returnType, name;
    vector<pair<string, string>> params;
    vector<unique_ptr<Stmt>> body;
public:
    FunctionDecl(const string& rt, const string& n,
                 vector<pair<string, string>> p,
                 vector<unique_ptr<Stmt>> b)
        : returnType(rt), name(n), params(move(p)), body(move(b)) {}
    void print(int indent = 0) const override {
        printIndent(indent);
        cout << "FunctionDecl: " << returnType << " " << name << "\n";
        printIndent(indent + 1);
        cout << "Parameters:\n";
        for (auto& param : params) {
            printIndent(indent + 2);
            cout << param.first << " " << param.second << "\n";
        }
        printIndent(indent + 1);
        cout << "Body:\n";
        for (auto& stmt : body) {
            stmt->print(indent + 2);
        }
    }
};

