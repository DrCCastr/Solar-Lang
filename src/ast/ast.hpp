#pragma once

#include "lexer/lexer.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <memory>

enum class NodeType {
    Program,
    NumericLiteral,
    Identifier,
    BinaryExpr
};

class Stmt {
public:
    virtual ~Stmt() = default;
    virtual NodeType getKind() const = 0;
    virtual std::string toString(int indentLevel = 0) const = 0;

protected:
    std::string indent(int level) const {
        return std::string(level * 2, ' ');
    }
};

class Program : public Stmt {
public:
    NodeType getKind() const override { return NodeType::Program; }
    std::vector<std::unique_ptr<Stmt>> body;
    std::string toString(int indentLevel = 0) const override {
        std::string result;
        result += indent(indentLevel) + "Program\n";
        result += indent(indentLevel) + "Body: {\n";
        for (const auto& stmt : body) {
            result += stmt->toString(indentLevel + 1) + "\n";
        }
        result += indent(indentLevel) + "}";
        return result;
    }
};

class Expr : public Stmt {};

class NumericLiteral : public Expr {
public:
    NumericLiteral(double value) : value(value) {}

    NodeType getKind() const override { return NodeType::NumericLiteral; }
    std::string toString(int indentLevel = 0) const override {
        return indent(indentLevel) + "NumericLiteral: " + std::to_string(value);
    }

    double value;
};

class Identifier : public Expr {
public:
    Identifier(std::string name) : name(std::move(name)) {}

    NodeType getKind() const override { return NodeType::Identifier; }
    std::string toString(int indentLevel = 0) const override {
        return indent(indentLevel) + "Identifier: " + name;
    }

    std::string name;
};

class BinaryExpr : public Expr {
public:
    BinaryExpr(std::unique_ptr<Expr> left, std::string op, std::unique_ptr<Expr> right)
        : left(std::move(left)), right(std::move(right)), op(std::move(op)) {}

    NodeType getKind() const override { return NodeType::BinaryExpr; }
    std::string toString(int indentLevel = 0) const override {
        std::string result = indent(indentLevel) + "BinaryExpr: {\n";
        result += left->toString(indentLevel + 1) + "\n";
        result += indent(indentLevel + 1) + "Operator: " + op + "\n";
        result += right->toString(indentLevel + 1) + "\n";
        result += indent(indentLevel) + "}";
        return result;
    }

    std::unique_ptr<Expr> left;
    std::unique_ptr<Expr> right;
    std::string op;
};

class Parser {
public:
    std::unique_ptr<Program> produceAST(std::string sourceCode);

private:
    Token next();
    Token expect(TokenEnum expected, std::string err = "");
    const Token& actual();
    bool notEOF() const;

    std::unique_ptr<Stmt> parseStmt();
    std::unique_ptr<Expr> parseExpr();
    std::unique_ptr<Expr> parseAddtiveExpr();
    std::unique_ptr<Expr> parseMultiplicativeExpr();
    std::unique_ptr<Expr> parsePrimaryExpr();

    std::vector<Token> tokens;
};
