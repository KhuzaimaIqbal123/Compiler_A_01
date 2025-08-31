#pragma once
#include "token.hpp"
#include <vector>
#include <regex>
#include <fstream>
#include <sstream>

using namespace std;

class Lexer {
    string source;
    size_t pos = 0;

public:
    Lexer(const string& filename) {
        ifstream file(filename);
        stringstream buffer;
        buffer << file.rdbuf();
        source = buffer.str();
    }

    vector<Token> tokenize() {
        vector<Token> tokens;

        // Unicode-aware identifier regex
        regex id(R"([A-Za-z_\p{L}][A-Za-z0-9_\p{L}]*)", regex::optimize | regex::ECMAScript);
        regex num(R"([0-9]+(\.[0-9]+)?)");
        regex str(R"("([^"\\]|\\.)*")");
        regex ws(R"(\s+)");
        regex comment(R"(//.*|/\*[\s\S]*?\*/)");

        while (pos < source.size()) {
            string rem = source.substr(pos);
            smatch m;

            // --- Skip whitespace ---
            if (regex_search(rem, m, ws) && m.position() == 0) {
                pos += m.length();
                continue;
            }

            // --- Comments ---
            if (regex_search(rem, m, comment) && m.position() == 0) {
                tokens.push_back(Token(TokenType::T_COMMENT, m.str()));
                pos += m.length();
                continue;
            }

            // --- String literals ---
            if (regex_search(rem, m, str) && m.position() == 0) {
                string val = m.str();
                tokens.push_back(Token(TokenType::T_QUOTES, "\""));
                tokens.push_back(Token(TokenType::T_STRINGLIT, val.substr(1, val.size() - 2)));
                tokens.push_back(Token(TokenType::T_QUOTES, "\""));
                pos += m.length();
                continue;
            }

            // --- Numbers ---
            if (regex_search(rem, m, num) && m.position() == 0) {
                string val = m.str();
                if (val.find('.') != string::npos)
                    tokens.push_back(Token(TokenType::T_FLOATLIT, val));
                else
                    tokens.push_back(Token(TokenType::T_INTLIT, val));
                pos += m.length();
                continue;
            }

            // --- Identifiers / Keywords ---
            if (regex_search(rem, m, id) && m.position() == 0) {
                string val = m.str();

                // --- Check invalid identifier (starts with digit) ---
                if (isdigit(val[0])) {
                    tokens.push_back(Token(TokenType::T_ERROR, val));
                    pos += m.length();
                    continue;
                }

                if (val == "fn") tokens.push_back(Token(TokenType::T_FUNCTION));
                else if (val == "int") tokens.push_back(Token(TokenType::T_INT));
                else if (val == "float") tokens.push_back(Token(TokenType::T_FLOAT));
                else if (val == "string") tokens.push_back(Token(TokenType::T_STRING));
                else if (val == "bool") tokens.push_back(Token(TokenType::T_BOOL));
                else if (val == "return") tokens.push_back(Token(TokenType::T_RETURN));
                else if (val == "if") tokens.push_back(Token(TokenType::T_IF));
                else if (val == "else") tokens.push_back(Token(TokenType::T_ELSE));
                else if (val == "for") tokens.push_back(Token(TokenType::T_FOR));
                else if (val == "while") tokens.push_back(Token(TokenType::T_WHILE));
                else if (val == "true") tokens.push_back(Token(TokenType::T_TRUE));
                else if (val == "false") tokens.push_back(Token(TokenType::T_FALSE));
                else tokens.push_back(Token(TokenType::T_IDENTIFIER, val));
                pos += m.length();
                continue;
            }

            // --- Operators & Symbols ---
            char c = rem[0];
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
                    if (rem.size() > 1 && rem[1] == '=') { tokens.push_back(Token(TokenType::T_EQUALSOP)); pos++; }
                    else tokens.push_back(Token(TokenType::T_ASSIGNOP));
                    break;
                case '!':
                    if (rem.size() > 1 && rem[1] == '=') { tokens.push_back(Token(TokenType::T_NOTEQ)); pos++; }
                    else tokens.push_back(Token(TokenType::T_ERROR, "!"));
                    break;
                case '<':
                    if (rem.size() > 1 && rem[1] == '=') { tokens.push_back(Token(TokenType::T_LTE)); pos++; }
                    else tokens.push_back(Token(TokenType::T_LT));
                    break;
                case '>':
                    if (rem.size() > 1 && rem[1] == '=') { tokens.push_back(Token(TokenType::T_GTE)); pos++; }
                    else tokens.push_back(Token(TokenType::T_GT));
                    break;
                case '&':
                    if (rem.size() > 1 && rem[1] == '&') { tokens.push_back(Token(TokenType::T_AND)); pos++; }
                    else tokens.push_back(Token(TokenType::T_ERROR, "&"));
                    break;
                case '|':
                    if (rem.size() > 1 && rem[1] == '|') { tokens.push_back(Token(TokenType::T_OR)); pos++; }
                    else tokens.push_back(Token(TokenType::T_ERROR, "|"));
                    break;
                default:
                    tokens.push_back(Token(TokenType::T_ERROR, string(1, c)));
            }
            pos++;
        }

        tokens.push_back(Token(TokenType::T_EOF));
        return tokens;
    }
};
