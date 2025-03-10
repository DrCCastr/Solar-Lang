/***
 * @file parser.cpp
 * @brief Implements the Parser class.
 */

#include "parser.hpp"

/***
 * @brief Consumes the actual token.
 * @return Consumed token.
 */
Token Parser::next() {
    Token tk = this->tokens.at(0);
    this->tokens.erase(this->tokens.begin());
    return tk;
}

/***
 * @brief Expects a token and returns it if it is the expected one.
 * @param expected The expected token.
 * @param err The error message to display if the token is not the expected one.
 * @return The expected token.
 */
Token Parser::expect(TokenEnum expected, std::string err) {
    Token tk = this->next();

    if (tk.type != expected) {
        throw Error::SyntaxError(
            "Expected: " + 
            TokenEnumString(expected) + 
            "at " + tk.pos.toString() +  
            ", Got: " + TokenEnumString(tk.type) + 
            err
        );
    }

    return tk;
}

/***
 * @brief Returns the actual token.
 * @return The actual token.
 */
const Token& Parser::actual() {
    return this->tokens.at(0);
}

/***
 * @brief Checks if the token list is not empty.
 * @return True if the token list is not empty, false otherwise.
 */
bool Parser::notEOF() const {
    return !this->tokens.empty() && this->tokens.at(0).type != TokenEnum::FE;
}

/***
 * @brief Parses a list of tokens and returns the AST.
 * @param tokens The list of tokens to parse.
 * @return The AST.
 */
std::unique_ptr<Program> Parser::produceAST(std::vector<Token> tokens) {
    this->tokens = tokens;
    auto program = std::make_unique<Program>(NodePos(0, 0, 0, 0));

    while (this->notEOF()) {
        program->body.push_back(this->parseStmt());
    }

    return program;
}

///////////////
// Statments //
///////////////

/***
 * @brief Parses a statement and returns it.
 * @return The statement.
 */
std::unique_ptr<Stmt> Parser::parseStmt() {
    switch (this->actual().type) {
        case TokenEnum::Var: return this->parseVarDeclaration();

        default: return this->parseExpr(); break;
    }
}

/***
 * @brief Parse a variable declarationa nd returns it.
 * @return The var declaration
 */
std::unique_ptr<Stmt> Parser::parseVarDeclaration() {
    auto startPos = this->next().pos;

    auto ident = this->expect(TokenEnum::Ident).content;
    std::unique_ptr<Expr> value;

    if (this->actual().type != TokenEnum::Semicolon) {
        this->expect(TokenEnum::Equals);

        value = this->parseExpr();

        this->expect(TokenEnum::Semicolon);
    } else {
        this->next();
    }

    return std::make_unique<VarDeclaration>(std::move(value), ident, NodePos::combineTP(startPos, startPos));
}

/////////////////
// Expressions //
/////////////////

/***
 * @brief Parses an expression and returns it.
 * @return The expression.
 */
std::unique_ptr<Expr> Parser::parseExpr() {
    return this->parseAddtiveExpr();
}

/***
 * @brief Parses an additive expression and returns it.
 * @return The additive expression.
 */
std::unique_ptr<Expr> Parser::parseAddtiveExpr() {
    auto left = this->parseMultiplicativeExpr();

    while (this->notEOF() && (this->actual().content == "+" || this->actual().content == "-")) {
        std::string op = this->next().content;
        auto right = this->parseMultiplicativeExpr();
        left = std::make_unique<BinaryExpr>(std::move(left), op, std::move(right), NodePos::combineNP(left->position, right->position));
    }

    return left;
}

/***
 * @brief Parses a multiplicative expression and returns it.
 * @return The multiplicative expression.
 */
std::unique_ptr<Expr> Parser::parseMultiplicativeExpr() {
    auto left = this->parsePrimaryExpr();

    while (this->notEOF() && (this->actual().content == "*" || this->actual().content == "/" || this->actual().content == "%")) {
        std::string op = this->next().content;
        auto right = this->parsePrimaryExpr();
        left = std::make_unique<BinaryExpr>(std::move(left), op, std::move(right), NodePos::combineNP(left->position, right->position));
    }

    return left;
}

/***
 * @brief Parses a primary expression and returns it.
 * @return The primary expression.
 */
std::unique_ptr<Expr> Parser::parsePrimaryExpr() {
    if (this->tokens.empty()) return nullptr;

    TokenEnum tkType = this->actual().type;
    TokenPos tkPos = this->actual().pos;
    NodePos ndPos = NodePos(tkPos.line, tkPos.start, tkPos.end, tkPos.line);

    switch (tkType) {
        case TokenEnum::Ident: return std::make_unique<Identifier>(this->next().content, ndPos); break;
        case TokenEnum::Number: return std::make_unique<NumericLiteral>(std::stoi(this->next().content), ndPos); break;
        case TokenEnum::LParam: {
            this->next();
            auto expr = this->parseExpr();
            this->expect(TokenEnum::RParam);
            return expr;
            break;
        }

        default:
            throw Error::SyntaxError(
                "Unexpected token while parsing at: " + 
                this->actual().pos.toString() + 
                ", Token: " + this->actual().content
            );
            break;
    }
}