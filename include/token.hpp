#pragma once
#include <string>
#include <iostream>

enum class TokenType {
    
    // Keywords
    T_FUNCTION, T_INT, T_FLOAT, T_STRING, T_BOOL, T_RETURN,
    T_IF, T_ELSE, T_FOR, T_WHILE, T_TRUE, T_FALSE,

    // Identifiers & literals
    T_IDENTIFIER, T_INTLIT, T_FLOATLIT, T_STRINGLIT,

    // Operators
    T_ASSIGNOP, T_EQUALSOP, T_NOTEQ, T_LT, T_GT, T_LTE, T_GTE,
    T_AND, T_OR, T_PLUS, T_MINUS, T_MUL, T_DIV, T_MOD,

    // Separators
    T_PARENL, T_PARENR, T_BRACEL, T_BRACER, T_BRACKETL, T_BRACKETR,
    T_COMMA, T_SEMICOLON, T_QUOTES,

    // Comments
    T_COMMENT,

    // Errors
    T_ERROR,

    // End of file
    T_EOF
};

struct Token {
    TokenType type;
    std::string value;

    Token(TokenType t, const std::string& v = "") : type(t), value(v) {}

    void print() const {
        switch (type) {
            case TokenType::T_IDENTIFIER: std::cout << "T_IDENTIFIER(\"" << value << "\")"; break;
            case TokenType::T_INTLIT: std::cout << "T_INTLIT(" << value << ")"; break;
            case TokenType::T_FLOATLIT: std::cout << "T_FLOATLIT(" << value << ")"; break;
            case TokenType::T_STRINGLIT: std::cout << "T_STRINGLIT(\"" << value << "\")"; break;
            case TokenType::T_ERROR: std::cout << "T_ERROR(" << value << ")"; break;
            default:
                std::cout << tokenName(type);
        }
    }

    static std::string tokenName(TokenType t) {
        switch (t) {
            case TokenType::T_FUNCTION: return "T_FUNCTION";
            case TokenType::T_INT: return "T_INT";
            case TokenType::T_FLOAT: return "T_FLOAT";
            case TokenType::T_STRING: return "T_STRING";
            case TokenType::T_BOOL: return "T_BOOL";
            case TokenType::T_RETURN: return "T_RETURN";
            case TokenType::T_IF: return "T_IF";
            case TokenType::T_ELSE: return "T_ELSE";
            case TokenType::T_FOR: return "T_FOR";
            case TokenType::T_WHILE: return "T_WHILE";
            case TokenType::T_TRUE: return "T_TRUE";
            case TokenType::T_FALSE: return "T_FALSE";
            case TokenType::T_ASSIGNOP: return "T_ASSIGNOP";
            case TokenType::T_EQUALSOP: return "T_EQUALSOP";
            case TokenType::T_NOTEQ: return "T_NOTEQ";
            case TokenType::T_LT: return "T_LT";
            case TokenType::T_GT: return "T_GT";
            case TokenType::T_LTE: return "T_LTE";
            case TokenType::T_GTE: return "T_GTE";
            case TokenType::T_AND: return "T_AND";
            case TokenType::T_OR: return "T_OR";
            case TokenType::T_PLUS: return "T_PLUS";
            case TokenType::T_MINUS: return "T_MINUS";
            case TokenType::T_MUL: return "T_MUL";
            case TokenType::T_DIV: return "T_DIV";
            case TokenType::T_MOD: return "T_MOD";
            case TokenType::T_PARENL: return "T_PARENL";
            case TokenType::T_PARENR: return "T_PARENR";
            case TokenType::T_BRACEL: return "T_BRACEL";
            case TokenType::T_BRACER: return "T_BRACER";
            case TokenType::T_BRACKETL: return "T_BRACKETL";
            case TokenType::T_BRACKETR: return "T_BRACKETR";
            case TokenType::T_COMMA: return "T_COMMA";
            case TokenType::T_SEMICOLON: return "T_SEMICOLON";
            case TokenType::T_QUOTES: return "T_QUOTES";
            case TokenType::T_COMMENT: return "T_COMMENT";
            case TokenType::T_EOF: return "T_EOF";
            default: return "UNKNOWN";
        }
    }
};
