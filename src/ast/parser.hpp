/***
 * @file parser.hpp
 * @brief Define the Parser class.
 */

#pragma once

#include "lexer/tokens.hpp"
#include "nodes.hpp"
#include "ast_error.hpp"

/***
 * @class Parser
 * @brief Parser class

 * @property tokens The list of tokens to parse.
 */
class Parser {
public:
    std::unique_ptr<Program> produceAST(std::vector<Token> tokens);

private:
    /*** * @brief Consumes the actual token and returns the next one. */
    Token next();
    /*** * @brief Expects a token and returns it if it is the expected one. */
    Token expect(TokenEnum expected, std::string err = ", Invalid expression.");
    /*** * @brief Returns the actual token. */
    const Token& actual();
    /*** * @brief Checks if the token list is not empty. */
    bool notEOF() const;

    std::unique_ptr<Stmt> parseStmt();
    std::unique_ptr<Expr> parseExpr();
    std::unique_ptr<Expr> parseAddtiveExpr();
    std::unique_ptr<Expr> parseMultiplicativeExpr();
    std::unique_ptr<Expr> parsePrimaryExpr();

    std::vector<Token> tokens;
};