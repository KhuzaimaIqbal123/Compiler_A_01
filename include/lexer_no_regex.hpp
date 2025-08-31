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
