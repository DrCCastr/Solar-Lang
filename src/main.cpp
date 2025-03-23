/***
 * @file main.cpp
 */

//////////////
// Includes //
//////////////

#include "solar_pack.hpp"
#include <stdexcept>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

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
    Solar::Parser parser;

    auto result = parser.parseCode(readFile("../test/script.sun"), "script.sun");
    cout << result->debug() << endl;

    return 0;
}