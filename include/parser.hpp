#pragma once
#include "token.hpp"
#include "ast.hpp"
#include <vector>
#include <memory>
#include <stdexcept>
using namespace std;

class Parser {
    vector<Token> tokens;
    size_t pos = 0;

    Token peek() const {
        if (pos < tokens.size()) return tokens[pos];
        return Token(TokenType::T_EOF);
    }

    Token get() {
        if (pos < tokens.size()) return tokens[pos++];
        return Token(TokenType::T_EOF);
    }

    bool match(TokenType type) {
        if (peek().type == type) {
            get();
            return true;
        }
        return false;
    }

    void expect(TokenType type, const string& msg) {
        if (!match(type)) {
            throw runtime_error("Expected " + msg + " at token: " + peek().value);
        }
    }

public:
    Parser(const vector<Token>& t) : tokens(t) {}

    unique_ptr<Program> parseProgram();

private:
    unique_ptr<Stmt> parseStatement();
    unique_ptr<FunctionDecl> parseFunctionDecl();
    unique_ptr<VarDecl> parseVarDecl();
    unique_ptr<ReturnStmt> parseReturnStmt();
    unique_ptr<Expr> parseExpression();
    unique_ptr<Expr> parsePrimary();
};
