#ifndef PARSER_HPP
#define PARSER_HPP

#include "token.hpp"
#include "ast.hpp"
#include <vector>
#include <memory>
using namespace std;

class Parser {
    vector<Token> tokens;   // list of tokens
    size_t pos = 0;         // current index

public:
    Parser(const vector<Token>& toks) : tokens(toks) {}

    // parse the whole program
    unique_ptr<Program> parseProgram();

private:
    // helper methods
    bool match(TokenType type);
    Token consume(TokenType type, const string& msg);
    Token peek();

    // statement parsing
    unique_ptr<Stmt> parseFunctionDecl();
    unique_ptr<Stmt> parseVarDecl();
    unique_ptr<Stmt> parseReturnStmt();

    // expression parsing
    unique_ptr<Expr> parseExpression();
    unique_ptr<Expr> parsePrimary();
};

#endif // PARSER_HPP
