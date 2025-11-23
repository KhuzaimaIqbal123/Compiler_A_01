#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <stdexcept>
#include <cctype>
#include <fstream>
#include <sstream>

inline bool isAlpha(char c) {
    return std::isalpha(static_cast<unsigned char>(c));
}

inline bool isDigit(char c) {
    return std::isdigit(static_cast<unsigned char>(c));
}

inline bool isAlphaNumeric(char c) {
    return std::isalnum(static_cast<unsigned char>(c));
}

inline bool isSpace(char c) {
    return std::isspace(static_cast<unsigned char>(c));
}

inline std::string read_file(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Could not open file: " + filename);

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

inline void report_error(const std::string &filename,
                         const std::string &src,
                         int line,
                         int col,
                         const std::string &msg) 
{
    throw std::runtime_error(
        filename + ":" + std::to_string(line) + ":" + std::to_string(col) + " -> " + msg
    );
}

#endif // UTILS_HPP
