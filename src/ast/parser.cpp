#include "parser.hpp"

Token Parser::next() {
    Token tk = this->tokens.at(0);
    this->tokens.erase(this->tokens.begin());
    return tk;
}

Token Parser::expect(TokenEnum expected, std::string err) {
    Token tk = this->next();

    if (tk.type != expected) {
        throw SolarError::SyntaxError(
            "Expected: " + 
            TokenEnumString(expected) + 
            "at " + tk.pos.toString() +  
            ", Got: " + TokenEnumString(tk.type) + 
            err
        );
    }

    return tk;
}

const Token& Parser::actual() {
    return this->tokens.at(0);
}

bool Parser::notEOF() const {
    return !this->tokens.empty() && this->tokens.at(0).type != TokenEnum::FE;
}

std::unique_ptr<Program> Parser::produceAST(std::vector<Token> tokens) {
    this->tokens = tokens;
    auto program = std::make_unique<Program>(NodePos{0, 0, 0, 0});

    while (this->notEOF()) {
        program->body.push_back(this->parseStmt());
    }

    return program;
}

std::unique_ptr<Stmt> Parser::parseStmt() {
    return this->parseExpr();
}

std::unique_ptr<Expr> Parser::parseExpr() {
    return this->parseAddtiveExpr();
}

std::unique_ptr<Expr> Parser::parseAddtiveExpr() {
    auto left = this->parseMultiplicativeExpr();

    while (this->notEOF() && (this->actual().content == "+" || this->actual().content == "-")) {
        std::string op = this->next().content;
        auto right = this->parseMultiplicativeExpr();
        left = std::make_unique<BinaryExpr>(std::move(left), op, std::move(right), NodePos().combineNP(left->position, right->position));
    }

    return left;
}

std::unique_ptr<Expr> Parser::parseMultiplicativeExpr() {
    auto left = this->parsePrimaryExpr();

    while (this->notEOF() && (this->actual().content == "*" || this->actual().content == "/" || this->actual().content == "%")) {
        std::string op = this->next().content;
        auto right = this->parsePrimaryExpr();
        left = std::make_unique<BinaryExpr>(std::move(left), op, std::move(right), NodePos().combineNP(left->position, right->position));
    }

    return left;
}

std::unique_ptr<Expr> Parser::parsePrimaryExpr() {
    if (this->tokens.empty()) return nullptr;

    TokenEnum tkType = this->actual().type;
    TokenPos tkPos = this->actual().pos;
    NodePos ndPos = NodePos{tkPos.line, tkPos.start, tkPos.end, tkPos.line};

    switch (tkType) {
        case TokenEnum::Ident: return std::make_unique<Identifier>(this->next().content, ndPos); break;

        case TokenEnum::Null: this->next(); return std::make_unique<NullLiteral>(ndPos); break;
        case TokenEnum::Number: return std::make_unique<NumericLiteral>(std::stoi(this->next().content), ndPos); break;

        case TokenEnum::LParam: {
            this->next();
            auto expr = this->parseExpr();
            this->expect(TokenEnum::RParam);
            return expr;
            break;
        }

        default:
            throw SolarError::SyntaxError(
                "Unexpected token while parsing at: " + 
                this->actual().pos.toString() + 
                ", Token: " + this->actual().content
            );
            break;
    }
}