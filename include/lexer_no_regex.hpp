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
    
