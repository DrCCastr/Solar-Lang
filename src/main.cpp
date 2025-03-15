/***
 * @file main.cpp
 */

//////////////
// Includes //
//////////////

#include "lexer/lexer.hpp"
#include <stdexcept>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

//////////
// Code //
//////////

std::string readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Cant open the file: " + path);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main()
{
    auto tokens = Solar::Lexer::tokenize(readFile("../test/script.sun"));

    for (const auto& token : tokens) {
        std::cout << token.content << " : " << Solar::Lexer::TToString(token.type) << " " << token.pos.toString() << std::endl;
    }

    return 0;
}