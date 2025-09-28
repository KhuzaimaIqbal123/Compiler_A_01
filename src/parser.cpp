#include "../include/parser.hpp"

unique_ptr<Program> Parser::parseProgram() {
    vector<unique_ptr<Stmt>> stmts;
    while (peek().type != TokenType::T_EOF) {
        stmts.push_back(parseStatement());
    }
    return make_unique<Program>(move(stmts));
}

unique_ptr<Stmt> Parser::parseStatement() {
    Token t = peek();
    if (t.type == TokenType::T_FUNCTION) return parseFunctionDecl();
    if (t.type == TokenType::T_INT || t.type == TokenType::T_FLOAT || 
        t.type == TokenType::T_STRING || t.type == TokenType::T_BOOL)
        return parseVarDecl();
    if (t.type == TokenType::T_RETURN) return parseReturnStmt();
    throw runtime_error("Unexpected statement at token: " + t.value);
}

unique_ptr<FunctionDecl> Parser::parseFunctionDecl() {
    expect(TokenType::T_FUNCTION, "fn keyword");

    Token retType = get(); 
    string returnType = retType.value;

    Token name = get();
    if (name.type != TokenType::T_IDENTIFIER)
        throw runtime_error("Expected function name");

    expect(TokenType::T_PARENL, "(");

    vector<pair<string,string>> params;
    while (peek().type != TokenType::T_PARENR) {
        Token type = get(); 
        Token id = get();   
        params.push_back({type.value, id.value});
        if (!match(TokenType::T_COMMA)) break;
    }

    expect(TokenType::T_PARENR, ")");
    expect(TokenType::T_BRACEL, "{");

    vector<unique_ptr<Stmt>> body;
    while (peek().type != TokenType::T_BRACER) {
        body.push_back(parseStatement());
    }

    expect(TokenType::T_BRACER, "}");

    return make_unique<FunctionDecl>(returnType, name.value, move(params), move(body));
}

unique_ptr<VarDecl> Parser::parseVarDecl() {
    Token type = get(); 
    Token name = get();
    if (name.type != TokenType::T_IDENTIFIER)
        throw runtime_error("Expected variable name");

    unique_ptr<Expr> init = nullptr;
    if (match(TokenType::T_ASSIGNOP)) {
        init = parseExpression();
    }

    expect(TokenType::T_SEMICOLON, ";");

    return make_unique<VarDecl>(type.value, name.value, move(init));
}

unique_ptr<ReturnStmt> Parser::parseReturnStmt() {
    expect(TokenType::T_RETURN, "return");
    auto expr = parseExpression();
    expect(TokenType::T_SEMICOLON, ";");
    return make_unique<ReturnStmt>(move(expr));
}
