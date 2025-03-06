#pragma once

#include "lexer/tokens.hpp"
#include "nodes.hpp"
#include "ast_error.hpp"

class Parser {
public:
    std::unique_ptr<Program> produceAST(std::vector<Token> tokens);

private:
    Token next();
    Token expect(TokenEnum expected, std::string err = ", Invalid expression.");
    const Token& actual();
    bool notEOF() const;

    std::unique_ptr<Stmt> parseStmt();
    std::unique_ptr<Expr> parseExpr();
    std::unique_ptr<Expr> parseAddtiveExpr();
    std::unique_ptr<Expr> parseMultiplicativeExpr();
    std::unique_ptr<Expr> parsePrimaryExpr();

    std::vector<Token> tokens;
};