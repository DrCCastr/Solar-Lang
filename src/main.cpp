/***
 * @file main.cpp
 * @brief Main file of the project
 */

#include "solar_lib.hpp"
#include <stdexcept>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

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
    try {
        Env env;
        Parser parser;
        std::string content = readFile("../test/script.sun");

        auto tokens = lexerParse(content);

        std::unique_ptr<Stmt> program = parser.produceAST(tokens);
        std::cout << program->toString() << std::endl;

        auto result = evaluateNode(std::move(program), env);

        std::cout << std::visit([](auto&& arg) { return arg.toString(); }, result) << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}