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

using namespace std;
using namespace Solar;

//////////
// Code //
//////////

string readFile(const string& path) {
    ifstream file(path);
    if (!file.is_open()) {
        throw runtime_error("Cant open the file: " + path);
    }

    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main()
{
    auto tokens = Lexer::tokenize(readFile("../test/script.sun"));

    for (const auto& token : tokens) {
        cout << token.content << " : " << Lexer::TToString(token.type) << " " << token.pos.toString() << endl;
    }

    return 0;
}