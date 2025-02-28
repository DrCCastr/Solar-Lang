#include "ast/ast.hpp"
#include <string>
#include <iostream>

#define Content "(1 + x) * b"

int main()
{
    Parser parser;

    auto program = parser.produceAST(Content);

    std::cout << program->toString() << std::endl;

    return 0;
}