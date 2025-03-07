// Calculate a expression with sun interpreter

#include "solar_lib.hpp"

// Expression example
#define Content "1+2*3"

int main() {
    // Creates a new instace of the parser
    Parser parser;

    auto tokens = lexerParse(Content); // Generate tokens
    std::unique_ptr<Stmt> program = parser.produceAST(tokens); // Generate AST
    auto result = evaluateNode(std::move(program)); // Evaluate the AST

    std::cout << result->toString() << std::endl; // Print the result
    /*
    Example output:
        7
    */

    return 0;
}