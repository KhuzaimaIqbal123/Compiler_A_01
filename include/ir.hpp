#ifndef IR_H
#define IR_H

#include <string>
#include <vector>
#include <iostream>
#include "parser.h"

using std::string;
using std::vector;

enum class IROp {
    ADD, SUB, MUL, DIV, MOD,
    LSHIFT, RSHIFT,
    LT, GT, LTE, GTE, EQ, NEQ,
    BIT_AND, BIT_OR, BIT_XOR,
    LOG_AND, LOG_OR,
    ASSIGN,
    LABEL,
    JMP,
    JMP_FALSE,
    PARAM,
    CALL,
    RET,
    FUNC_START,
    FUNC_END
};

struct Quad {
    IROp op;
    string arg1;
    string arg2;
    string result;

    string toString() const;
};

class IRGenerator {
public:
    IRGenerator() = default;

    void generate(Program* program);
    void printInstructions() const;
    const vector<Quad>& getInstructions() const { return instructions; }

private:
    vector<Quad> instructions;
    int tempCounter = 0;
    int labelCounter = 0;

    string newTemp();
    string newLabel();
    void emit(IROp op, const string& arg1, const string& arg2, const string& result);

    void genStmt(Stmt* stmt);
    string genExpr(Expr* expr);

    void handleBlock(BlockStmt* stmt);
    void handleVarDecl(VarDecl* stmt);
    void handleIf(IfStmt* stmt);
    void handleWhile(WhileStmt* stmt);
    void handleFor(ForStmt* stmt);
    void handleReturn(ReturnStmt* stmt);
    void handleFuncDecl(FuncDecl* stmt);
    void handleExprStmt(ExprStmt* stmt);

    string handleBinary(BinaryExpr* expr);
    string handleUnary(UnaryExpr* expr);
    string handleLiteral(LiteralExpr* expr);
    string handleIdent(IdentExpr* expr);
    string handleCall(CallExpr* expr);
    string handleAssign(BinaryExpr* expr);

    IROp tokenToOp(TokenType type);
};

#endif
