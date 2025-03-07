#include "ast/ast.hpp"
#include "runtime/runner.hpp"
#include "lexer/tokenizer.hpp"
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
        Parser parser;
        std::string content = readFile("../test/script.sun");

        auto tokens = lexerParse(content);
        std::unique_ptr<Stmt> program = parser.produceAST(tokens);
        auto result = evaluateNode(std::move(program));

        std::cout << result->toString() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}