#pragma once
#include "token.hpp"
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

class LexerNoRegex {
    string source;
    size_t pos = 0;

public:
    
    //File Reading & Setup
    LexerNoRegex(const string& filename) {
        ifstream file(filename);
        stringstream buffer;
        buffer << file.rdbuf();
        source = buffer.str();
    }
 //Tokenization 
    
    vector<Token> tokenize() {
        vector<Token> tokens;

        while (pos < source.size()) {
            char c = source[pos];

            // Skip whitespace 
            if (c == ' ' || c == '\t' || c == '\n' || c == '\r') { pos++; continue; }

            //  Comments 
            if (c == '/' && pos + 1 < source.size()) {
                if (source[pos + 1] == '/') {
                    string comment;
                    while (pos < source.size() && source[pos] != '\n') comment += source[pos++];
                    tokens.push_back(Token(TokenType::T_COMMENT, comment));
                    continue;
                } else if (source[pos + 1] == '*') {
                    string comment = "/*";
                    pos += 2;
                    while (pos + 1 < source.size() && !(source[pos] == '*' && source[pos+1] == '/')) comment += source[pos++];
                    if (pos + 1 < source.size()) { comment += "*/"; pos += 2; }
                    tokens.push_back(Token(TokenType::T_COMMENT, comment));
                    continue;
                }
            }
     // Strings 
            if (c == '"') {
                string strlit = "";
                tokens.push_back(Token(TokenType::T_QUOTES, "\""));
                pos++;
                while (pos < source.size() && source[pos] != '"') {
                    if (source[pos] == '\\' && pos + 1 < source.size()) {
                        strlit += source[pos]; strlit += source[pos+1]; pos += 2;
                    } else strlit += source[pos++];
                }
                if (pos < source.size() && source[pos] == '"') pos++;
                tokens.push_back(Token(TokenType::T_STRINGLIT, strlit));
                tokens.push_back(Token(TokenType::T_QUOTES, "\""));
                continue;
            }
    // Numbers 
            if (isdigit(c)) {
                string num;
                bool isFloat = false;
                while (pos < source.size() && (isdigit(source[pos]) || source[pos] == '.')) {
                    if (source[pos] == '.') isFloat = true;
                    num += source[pos++];
                }
                if (isFloat) tokens.push_back(Token(TokenType::T_FLOATLIT, num));
                else tokens.push_back(Token(TokenType::T_INTLIT, num));
                continue;
            }
if (isalpha(c) || c == '_' || (unsigned char)c >= 128) {
                string word;
                if (isdigit(c)) { // Invalid identifier
                    string err;
                    while (pos < source.size() && (isalnum(source[pos]) || source[pos] == '_' || (unsigned char)source[pos]>=128))
                        err += source[pos++];
                    tokens.push_back(Token(TokenType::T_ERROR, err));
                    continue;
                }

                while (pos < source.size()) {
                    unsigned char cc = source[pos];
                    if (isalnum(cc) || cc == '_' || cc >= 128) word += source[pos++];
                    else break;
                }
    // --- Keyword check ---
                if (word == "fn") tokens.push_back(Token(TokenType::T_FUNCTION));
                else if (word == "int") tokens.push_back(Token(TokenType::T_INT));
                else if (word == "float") tokens.push_back(Token(TokenType::T_FLOAT));
                else if (word == "string") tokens.push_back(Token(TokenType::T_STRING));
                else if (word == "bool") tokens.push_back(Token(TokenType::T_BOOL));
                else if (word == "return") tokens.push_back(Token(TokenType::T_RETURN));
                else if (word == "if") tokens.push_back(Token(TokenType::T_IF));
                else if (word == "else") tokens.push_back(Token(TokenType::T_ELSE));
                else if (word == "for") tokens.push_back(Token(TokenType::T_FOR));
                else if (word == "while") tokens.push_back(Token(TokenType::T_WHILE));
                else if (word == "true") tokens.push_back(Token(TokenType::T_TRUE));
                else if (word == "false") tokens.push_back(Token(TokenType::T_FALSE));
                else tokens.push_back(Token(TokenType::T_IDENTIFIER, word));
                continue;
            }
 // --- Operators & Symbols ---
            switch (c) {
                case '(': tokens.push_back(Token(TokenType::T_PARENL)); break;
                case ')': tokens.push_back(Token(TokenType::T_PARENR)); break;
                case '{': tokens.push_back(Token(TokenType::T_BRACEL)); break;
                case '}': tokens.push_back(Token(TokenType::T_BRACER)); break;
                case '[': tokens.push_back(Token(TokenType::T_BRACKETL)); break;
                case ']': tokens.push_back(Token(TokenType::T_BRACKETR)); break;
                case ',': tokens.push_back(Token(TokenType::T_COMMA)); break;
                case ';': tokens.push_back(Token(TokenType::T_SEMICOLON)); break;
                case '+': tokens.push_back(Token(TokenType::T_PLUS)); break;
                case '-': tokens.push_back(Token(TokenType::T_MINUS)); break;
                case '*': tokens.push_back(Token(TokenType::T_MUL)); break;
                case '/': tokens.push_back(Token(TokenType::T_DIV)); break;
                case '%': tokens.push_back(Token(TokenType::T_MOD)); break;
                case '=':
                    if (pos + 1 < source.size() && source[pos+1]=='=') { tokens.push_back(Token(TokenType::T_EQUALSOP)); pos++; }
                    else tokens.push_back(Token(TokenType::T_ASSIGNOP));
                    break;
                case '!':
                    if (pos + 1 < source.size() && source[pos+1]=='=') { tokens.push_back(Token(TokenType::T_NOTEQ)); pos++; }
                    else tokens.push_back(Token(TokenType::T_ERROR, "!"));
                    break;
                case '<':
                    if (pos + 1 < source.size() && source[pos+1]=='=') { tokens.push_back(Token(TokenType::T_LTE)); pos++; }
                    else tokens.push_back(Token(TokenType::T_LT));
                    break;
                case '>':
                    if (pos + 1 < source.size() && source[pos+1]=='=') { tokens.push_back(Token(TokenType::T_GTE)); pos++; }
                    else tokens.push_back(Token(TokenType::T_GT));
                    break;
                case '&':
                    if (pos + 1 < source.size() && source[pos+1]=='&') { tokens.push_back(Token(TokenType::T_AND)); pos++; }
                    else tokens.push_back(Token(TokenType::T_ERROR, "&"));
                    break;
                case '|':
                    if (pos + 1 < source.size() && source[pos+1]=='|') { tokens.push_back(Token(TokenType::T_OR)); pos++; }
                    else tokens.push_back(Token(TokenType::T_ERROR, "|"));
                    break;
                default: tokens.push_back(Token(TokenType::T_ERROR, string(1,c)));
            }
            pos++;
        }

        tokens.push_back(Token(TokenType::T_EOF));
        return tokens;
    }
};
