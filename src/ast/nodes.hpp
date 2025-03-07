#pragma once

#include "lexer/tokens.hpp"
#include <string>
#include <vector>
#include <memory>

enum class NodeType {
    Program,

    NumericLiteral,
    NullLiteral,

    Identifier,

    BinaryExpr
};
struct NodePos {
    size_t line;
    size_t start;
    size_t end;
    size_t endLine;

    NodePos(size_t l = 0, size_t s = 0, size_t e = 0, size_t el = 0)
        : line(l), start(s), end(e), endLine(el) {}

    static NodePos combineTP(const TokenPos& start, const TokenPos& end) {
        return NodePos(start.line, start.start, end.end, end.line);
    }

    static NodePos combineNP(const NodePos& start, const NodePos& end) {
        return NodePos(start.line, start.start, end.end, end.endLine);
    }

    std::string toString() const {
        if (line == endLine) {
            return "L" + std::to_string(line) + ":(" + std::to_string(start) + "-" + std::to_string(end) + ")";
        }
        return "L" + std::to_string(line) + ":" + std::to_string(start) + " until L" + std::to_string(endLine) + ":" + std::to_string(end);
    }
};

class Stmt {
public:
    virtual ~Stmt() = default;
    virtual NodeType getKind() const = 0;
    virtual std::string toString(int indentLevel = 0) const = 0;
    NodePos position;
protected:
    std::string indent(int level) const {
        return std::string(level * 2, ' ');
    }
};

class Program : public Stmt {
public:
    Program(const NodePos& position = NodePos(0, 0, 0, 0)) {
        this->position = position;
    }

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
    NumericLiteral(double value, const NodePos& position) : value(value) {
        this->position = position;
    }

    NodeType getKind() const override { return NodeType::NumericLiteral; }
    std::string toString(int indentLevel = 0) const override {
        return indent(indentLevel) + "NumericLiteral: " + std::to_string(value);
    }

    double value;
};

class NullLiteral : public Expr {
public:
    NullLiteral(const NodePos& position) {
        this->position = position;
    }

    NodeType getKind() const override { return NodeType::NullLiteral; }
    std::string toString(int indentLevel = 0) const override {
        return indent(indentLevel) + "NullLiteral";
    }
};

class Identifier : public Expr {
public:
    Identifier(std::string name, const NodePos& position) : name(std::move(name)) {
        this->position = position;
    }

    NodeType getKind() const override { return NodeType::Identifier; }
    std::string toString(int indentLevel = 0) const override {
        return indent(indentLevel) + "Identifier: " + name;
    }

    std::string name;
};

class BinaryExpr : public Expr {
public:
    BinaryExpr(std::unique_ptr<Expr> left, std::string op, std::unique_ptr<Expr> right, const NodePos& position)
        : left(std::move(left)), right(std::move(right)), op(std::move(op)) {
        this->position = position;
        }

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