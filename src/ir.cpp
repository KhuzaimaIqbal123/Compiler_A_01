#include "../include/ir.h"
#include <iostream>
#include <sstream>

using namespace std;

string opToString(IROp op) {
    switch (op) {
        case IROp::ADD: return "+";
        case IROp::SUB: return "-";
        case IROp::MUL: return "*";
        case IROp::DIV: return "/";
        case IROp::MOD: return "%";
        case IROp::LT: return "<";
        case IROp::GT: return ">";
        case IROp::LTE: return "<=";
        case IROp::GTE: return ">=";
        case IROp::EQ: return "==";
        case IROp::NEQ: return "!=";
        case IROp::ASSIGN: return "=";
        case IROp::JMP: return "goto";
        case IROp::JMP_FALSE: return "ifFalse";
        case IROp::PARAM: return "param";
        case IROp::CALL: return "call";
        case IROp::RET: return "return";
        default: return "?";
    }
}

string Quad::toString() const {
    if (op == IROp::LABEL) return result + ":";
    if (op == IROp::FUNC_START) return "FUNC " + result + ":";
    if (op == IROp::FUNC_END) return "END FUNC " + result;
    if (op == IROp::JMP) return "goto " + result;
    if (op == IROp::JMP_FALSE) return "ifFalse " + arg1 + " goto " + result;
    if (op == IROp::ASSIGN) return result + " = " + arg1;
    if (op == IROp::CALL) return result + " = call " + arg1 + ", " + arg2;
    if (op == IROp::RET) return "return " + arg1;
    if (op == IROp::PARAM) return "param " + arg1;
    return result + " = " + arg1 + " " + opToString(op) + " " + arg2;
}

void IRGenerator::generate(Program* program) {
    for (auto& stmt : program->items) genStmt(stmt.get());
}

string IRGenerator::newTemp() {
    return "_t" + to_string(tempCounter++);
}

string IRGenerator::newLabel() {
    return "L" + to_string(labelCounter++);
}

void IRGenerator::emit(IROp op, const string& arg1, const string& arg2, const string& result) {
    instructions.push_back({op, arg1, arg2, result});
}

void IRGenerator::printInstructions() const {
    for (const auto& q : instructions) cout << q.toString() << "\n";
}

void IRGenerator::genStmt(Stmt* stmt) {
    if (auto s = dynamic_cast<BlockStmt*>(stmt)) handleBlock(s);
    else if (auto s = dynamic_cast<VarDecl*>(stmt)) handleVarDecl(s);
    else if (auto s = dynamic_cast<IfStmt*>(stmt)) handleIf(s);
    else if (auto s = dynamic_cast<WhileStmt*>(stmt)) handleWhile(s);
    else if (auto s = dynamic_cast<ForStmt*>(stmt)) handleFor(s);
    else if (auto s = dynamic_cast<ReturnStmt*>(stmt)) handleReturn(s);
    else if (auto s = dynamic_cast<FuncDecl*>(stmt)) handleFuncDecl(s);
    else if (auto s = dynamic_cast<ExprStmt*>(stmt)) handleExprStmt(s);
}

string IRGenerator::genExpr(Expr* expr) {
    if (auto e = dynamic_cast<BinaryExpr*>(expr)) return handleBinary(e);
    else if (auto e = dynamic_cast<UnaryExpr*>(expr)) return handleUnary(e);
    else if (auto e = dynamic_cast<LiteralExpr*>(expr)) return handleLiteral(e);
    else if (auto e = dynamic_cast<IdentExpr*>(expr)) return handleIdent(e);
    else if (auto e = dynamic_cast<CallExpr*>(expr)) return handleCall(e);
    return "";
}

void IRGenerator::handleBlock(BlockStmt* stmt) {
    for (auto& s : stmt->statements) genStmt(s.get());
}

void IRGenerator::handleVarDecl(VarDecl* stmt) {
    if (stmt->initializer) {
        string val = genExpr(stmt->initializer->get());
        emit(IROp::ASSIGN, val, "", stmt->name);
    }
}

void IRGenerator::handleIf(IfStmt* stmt) {
    string cond = genExpr(stmt->condition.get());
    string elseLabel = newLabel();
    string endLabel = newLabel();

    emit(IROp::JMP_FALSE, cond, "", stmt->elseBranch ? elseLabel : endLabel);

    handleBlock(stmt->thenBranch.get());
    emit(IROp::JMP, "", "", endLabel);

    if (stmt->elseBranch) {
        emit(IROp::LABEL, "", "", elseLabel);
        handleBlock(stmt->elseBranch.get());
    }

    emit(IROp::LABEL, "", "", endLabel);
}

void IRGenerator::handleWhile(WhileStmt* stmt) {
    string startLabel = newLabel();
    string endLabel = newLabel();

    emit(IROp::LABEL, "", "", startLabel);
    string cond = genExpr(stmt->condition.get());
    emit(IROp::JMP_FALSE, cond, "", endLabel);

    handleBlock(stmt->body.get());
    emit(IROp::JMP, "", "", startLabel);
    emit(IROp::LABEL, "", "", endLabel);
}

void IRGenerator::handleFor(ForStmt* stmt) {
    if (stmt->init) genStmt(stmt->init->get());

    string startLabel = newLabel();
    string endLabel = newLabel();

    emit(IROp::LABEL, "", "", startLabel);

    if (stmt->condition) {
        string cond = genExpr(stmt->condition->get());
        emit(IROp::JMP_FALSE, cond, "", endLabel);
    }

    handleBlock(stmt->body.get());

    if (stmt->post) genExpr(stmt->post->get());
    
    emit(IROp::JMP, "", "", startLabel);
    emit(IROp::LABEL, "", "", endLabel);
}

void IRGenerator::handleReturn(ReturnStmt* stmt) {
    if (stmt->expr) {
        string val = genExpr(stmt->expr->get());
        emit(IROp::RET, val, "", "");
    } else {
        emit(IROp::RET, "", "", "");
    }
}

void IRGenerator::handleFuncDecl(FuncDecl* stmt) {
    emit(IROp::FUNC_START, "", "", stmt->name);
    handleBlock(stmt->body.get());
    emit(IROp::FUNC_END, "", "", stmt->name);
}

void IRGenerator::handleExprStmt(ExprStmt* stmt) {
    genExpr(stmt->expr.get());
}

string IRGenerator::handleBinary(BinaryExpr* expr) {
    if (expr->op.type == T_ASSIGNOP) {
        if (auto ident = dynamic_cast<IdentExpr*>(expr->left.get())) {
            string rhs = genExpr(expr->right.get());
            emit(IROp::ASSIGN, rhs, "", ident->name);
            return ident->name;
        }
    }

    string left = genExpr(expr->left.get());
    string right = genExpr(expr->right.get());
    string result = newTemp();

    emit(tokenToOp(expr->op.type), left, right, result);
    return result;
}

string IRGenerator::handleUnary(UnaryExpr* expr) {
    string operand = genExpr(expr->right.get());
    string result = newTemp();

    IROp op = IROp::ADD;
    if (expr->op.type == T_MINUS) op = IROp::SUB;
    else if (expr->op.type == T_NOT) op = IROp::LOG_AND;

    emit(op, "0", operand, result);
    return result;
}

string IRGenerator::handleLiteral(LiteralExpr* expr) {
    return expr->value;
}

string IRGenerator::handleIdent(IdentExpr* expr) {
    return expr->name;
}

string IRGenerator::handleCall(CallExpr* expr) {
    for (auto& arg : expr->args) {
        string argName = genExpr(arg.get());
        emit(IROp::PARAM, argName, "", "");
    }

    string result = newTemp();
    string funcName = "unknown_func";

    if (auto id = dynamic_cast<IdentExpr*>(expr->callee.get())) funcName = id->name;
    
    emit(IROp::CALL, funcName, to_string(expr->args.size()), result);
    return result;
}

IROp IRGenerator::tokenToOp(TokenType type) {
    switch (type) {
        case T_PLUS: return IROp::ADD;
        case T_MINUS: return IROp::SUB;
        case T_MULT: return IROp::MUL;
        case T_DIV: return IROp::DIV;
        case T_MOD: return IROp::MOD;
        case T_LT: return IROp::LT;
        case T_GT: return IROp::GT;
        case T_LTE: return IROp::LTE;
        case T_GTE: return IROp::GTE;
        case T_EQUALSOP: return IROp::EQ;
        case T_NEQ: return IROp::NEQ;
        default: return IROp::ADD;
    }
}
