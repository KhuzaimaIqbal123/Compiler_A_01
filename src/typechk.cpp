// typechk.cpp
#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstring>
#include "../include/typechk.h"

class TypeChecker {
private:
    struct TypeRes {
        std::string type; // "int", "float", "bool", "string", "char", "void", or empty string for error
    };

    Scope* current_scope = nullptr;
    int errors = 0;

    static std::string canon_type(const std::string& t) {
        if (t.empty()) return "";
        if (t == "T_INT") return "int";
        if (t == "T_FLOAT") return "float";
        if (t == "T_BOOL") return "bool";
        if (t == "T_STRING") return "string";
        if (t == "T_CHAR") return "char";
        if (t == "T_VOID") return "void";
        return t;
    }

    void report(TypeChkError tc, const std::string& msg, int line) {
        ++errors;
        std::cerr << "[TypeError] (" << line << "): " << msg << std::endl;
    }

    static bool is_numeric(const std::string& t) {
        return t == "int" || t == "float";
    }

    static bool is_int(const std::string& t) { return t == "int"; }
    static bool is_bool(const std::string& t) { return t == "bool"; }

    TypeRes type_expr(Node* n) {
        if (!n) { report(TC_EmptyExpression, "Empty expression encountered", 0); return {""}; }

        switch (n->type) {
            case NODE_INT_LIT: return {"int"};
            case NODE_FLOAT_LIT: return {"float"};
            case NODE_BOOL_LIT: return {"bool"};
            case NODE_STRING_LIT: return {"string"};
            case NODE_CHAR_LIT: return {"char"};
            case NODE_IDENTIFIER: {
                Symbol* s = scope_lookup(current_scope, n->as.ident.name, SYMBOL_VAR);
                if (!s) {
                    report(TC_ErroneousVarDecl, "Use of undeclared variable '" + std::string(n->as.ident.name) + "'", n->line);
                    return {""};
                }
                return {canon_type(s->type)};
            }
            case NODE_BINARY: return type_binary(n);
            case NODE_ASSIGN: return type_assign(n);
            case NODE_CALL: return type_call(n);
            default:
                report(TC_ExpressionTypeMismatch, "Unhandled expression node in typechecker", n->line);
                return {""};
        }
    }

    TypeRes type_binary(Node* n) {
        TypeRes L = type_expr(n->as.binary.left);
        TypeRes R = type_expr(n->as.binary.right);
        std::string op;
        switch (n->as.binary.op) {
            case T_PLUS: op = "+"; break;
            case T_MINUS: op = "-"; break;
            case T_MULT: op = "*"; break;
            case T_DIV: op = "/"; break;
            case T_LT: case T_GT: case T_LE: case T_GE: case T_EQ: case T_NEQ: op = "cmp"; break;
        }

        if (L.type.empty() || R.type.empty()) {
            report(TC_ExpressionTypeMismatch, "Empty expression or previous error in binary operands", n->line);
            return {""};
        }

        if (op == "+" || op == "-" || op == "*" || op == "/") {
            if (!is_numeric(L.type) || !is_numeric(R.type)) {
                report(TC_AttemptedAddOpOnNonNumeric, "Arithmetic operator on non-numeric operands", n->line);
                return {""};
            }
            if (L.type == "float" || R.type == "float") return {"float"};
            return {"int"};
        }

        if (op == "cmp") {
            if ((is_numeric(L.type) && is_numeric(R.type)) || L.type == R.type) return {"bool"};
            report(TC_ExpressionTypeMismatch, "Comparison between incompatible types", n->line);
            return {""};
        }

        return {""};
    }

    TypeRes type_assign(Node* n) {
        if (n->as.assign.left->type != NODE_IDENTIFIER) {
            report(TC_ExpressionTypeMismatch, "Left-hand side of assignment must be an identifier", n->line);
            return {""};
        }

        std::string varname = n->as.assign.left->as.ident.name;
        Symbol* sym = scope_lookup(current_scope, varname.c_str(), SYMBOL_VAR);
        if (!sym) {
            report(TC_ErroneousVarDecl, "Assignment to undeclared variable '" + varname + "'", n->line);
            return {""};
        }

        TypeRes r = type_expr(n->as.assign.right);
        if (r.type.empty()) return r;

        std::string decl_t = canon_type(sym->type);
        std::string rhs_t = canon_type(r.type);

        if (decl_t.empty() || rhs_t.empty()) {
            report(TC_ExpressionTypeMismatch, "Unknown type in assignment", n->line);
            return {""};
        }

        if (decl_t == rhs_t) return {decl_t};
        if (decl_t == "float" && rhs_t == "int") return {"float"};

        report(TC_ExpressionTypeMismatch, "Type mismatch in assignment: '" + decl_t + "' := '" + rhs_t + "'", n->line);
        return {""};
    }

    TypeRes type_call(Node* n) {
        if (n->as.call.callee->type != NODE_IDENTIFIER) {
            report(TC_FnCallParamCount, "Function callee is not a simple identifier", n->line);
            return {""};
        }

        std::string fname = n->as.call.callee->as.ident.name;
        Symbol* fsym = scope_lookup(current_scope, fname.c_str(), SYMBOL_FN);
        if (!fsym) {
            report(TC_FnCallParamCount, "Call to undefined function '" + fname + "'", n->line);
            return {""};
        }

        if (fsym->param_count != n->as.call.arg_count) {
            report(TC_FnCallParamCount, "Parameter count mismatch in call to '" + fname + "'", n->line);
        }

        for (int i = 0; i < n->as.call.arg_count; ++i) {
            TypeRes argt = type_expr(n->as.call.args[i]);
            if (i >= fsym->param_count) continue;
            std::string expected = canon_type(fsym->param_types[i]);
            if (!argt.type.empty() && argt.type != expected && !(expected=="float" && argt.type=="int")) {
                report(TC_FnCallParamType, "Parameter type mismatch in call to '" + fname + "'", n->line);
            }
        }
        return {canon_type(fsym->type)};
    }

    void tc_stmt(Node* stmt) {
        if (!stmt) return;
        switch (stmt->type) {
            case NODE_VAR_DECL: {
                if (stmt->as.var_decl.init) {
                    TypeRes rv = type_expr(stmt->as.var_decl.init);
                    std::string decl_t = canon_type(stmt->as.var_decl.type_name);
                    if (!rv.type.empty() && decl_t != rv.type && !(decl_t=="float" && rv.type=="int")) {
                        report(TC_ErroneousVarDecl, "Variable declaration type incompatible with initializer", stmt->line);
                    }
                }
                if (current_scope) {
                    scope_add_var(current_scope, stmt->as.var_decl.name, stmt->as.var_decl.type_name, stmt->line);
                } else {
                    report(TC_ErroneousVarDecl, "No current scope while declaring variable", stmt->line);
                }
                break;
            }
            case NODE_EXPR_STMT: type_expr(stmt->as.expr_stmt.expr); break;
            case NODE_PRINT: type_expr(stmt->as.printstmt.expr); break;
            case NODE_RETURN: if (stmt->as.ret.expr) type_expr(stmt->as.ret.expr); break;
            case NODE_IF:
                if (!is_bool(type_expr(stmt->as.ifstmt.cond).type)) {
                    report(TC_NonBooleanCondStmt, "If condition must be boolean", stmt->line);
                }
                tc_stmt(stmt->as.ifstmt.then_branch);
                if (stmt->as.ifstmt.else_branch) tc_stmt(stmt->as.ifstmt.else_branch);
                break;
            case NODE_WHILE:
                if (!is_bool(type_expr(stmt->as.whilestmt.cond).type)) {
                    report(TC_NonBooleanCondStmt, "While condition must be boolean", stmt->line);
                }
                tc_stmt(stmt->as.whilestmt.body);
                break;
            case NODE_FOR:
                if (stmt->as.forstmt.init) type_expr(stmt->as.forstmt.init);
                if (stmt->as.forstmt.cond && !is_bool(type_expr(stmt->as.forstmt.cond).type))
                    report(TC_NonBooleanCondStmt, "For condition must be boolean", stmt->line);
                if (stmt->as.forstmt.update) type_expr(stmt->as.forstmt.update);
                tc_stmt(stmt->as.forstmt.body);
                break;
            case NODE_BLOCK: tc_block(stmt); break;
            default: break;
        }
    }

    void tc_block(Node* block) {
        if (!block) return;
        for (int i = 0; i < block->as.block.stmt_count; ++i)
            tc_stmt(block->as.block.stmts[i]);
    }

public:
    TypeChkReport type_check(Node* root, Scope* global) {
        errors = 0;
        current_scope = global;

        if (!root) return {0};

        if (root->type == NODE_BLOCK) {
            for (int i = 0; i < root->as.block.stmt_count; ++i) {
                Node* st = root->as.block.stmts[i];
                if (!st) continue;
                if (st->type == NODE_FN_DECL) continue;
                tc_stmt(st);
            }

            for (Symbol* s = global->symbols; s; s = s->next) {
                if (s->kind == SYMBOL_FN && s->fn_node) {
                    Scope* fnscope = scope_new(global);
                    for (int i = 0; i < s->param_count; ++i)
                        scope_add_var(fnscope, s->fn_node->as.fn_decl.param_names[i], s->param_types[i], s->fn_node->line);

                    Scope* prev = current_scope;
                    current_scope = fnscope;
                    tc_block(s->fn_node->as.fn_decl.body);
                    current_scope = prev;
                    scope_free(fnscope);
                }
            }
        } else {
            tc_stmt(root);
        }

        if (errors == 0) std::cout << "Type checking passed: 0 errors\n";
        else std::cout << "Type checking finished: " << errors << " error(s)\n";
        return {errors};
    }
};
