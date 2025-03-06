#include "ast/ast.hpp"
#include "runtime/runner.hpp"
#include "lexer/tokenizer.hpp"
#include <string>
#include <iostream>

#define Content "1 + 1"

int main()
{
    Parser parser;

    auto tokens = lexerParse(Content);
    std::unique_ptr<Stmt> program = parser.produceAST(tokens);
    auto result = evaluateNode(std::move(program));

    std::cout << result->toString() << std::endl;

    return 0;
}