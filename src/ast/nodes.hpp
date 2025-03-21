/***
 * @file nodes.hpp
 */

#pragma once

//////////////
// Includes //
//////////////

#include "solar_pack.hpp"
#include <string>
#include <memory>
#include <vector>

using namespace std;
using namespace Solar;

//////////
// Code //
//////////

namespace Solar {
namespace Ast {

    enum class NodeType {
        ExprStmt,
        BlockStmt,

        NumberExpr,
        CharExpr,
        StringExpr,
        IdentExpr,

        AssignmentExpr,
        BinaryExpr,
        LogicalExpr,
        ComparasonExpr,
    };

    class Stmt {
    public:
        virtual ~Stmt() = default;
        virtual string debug(int indent = 0) const = 0;
        virtual NodeType getKind() const = 0;
    };

    class Expr : public Stmt {
    public:
        virtual ~Expr() = default;
    };

    using StmtPtr = shared_ptr<Stmt>;
    using ExprPtr = shared_ptr<Expr>;

    // ----------|
    // Statments |
    // ----------|

    class ExprStmt : public Stmt {
    public:
        ExprPtr expr;

        ExprStmt(ExprPtr expr) : expr(expr) {}

        string debug(int indent = 0) const override {
            string result;
            result += string(indent * 2, ' ') + "ExprStmt:\n";
            result += expr->debug(indent + 1);
            return result;
        }

        NodeType getKind() const override { return NodeType::ExprStmt; }
    };

    class BlockStmt : public Stmt {
    public:
        vector<StmtPtr> body;

        BlockStmt(vector<StmtPtr> body) : body(body) {}

        string debug(int indent = 0) const override {
            string result;
            result += string(indent * 2, ' ') + "BlockStmt: {\n";
            for (const auto& stmt : body) {
                result += stmt->debug(indent + 1);
            }
            result += string(indent * 2, ' ') + "}\n";
            return result;
        }

        NodeType getKind() const override { return NodeType::BlockStmt; }
    };

    // ------------|
    // Expressions |
    // ------------|

    class NumberExpr : public Expr {
    public:
        double value;

        NumberExpr(double value) : value(value) {}

        string debug(int indent = 0) const override {
            return string(indent * 2, ' ') + "NumberExpr: " + to_string(value) + "\n";
        }

        NodeType getKind() const override { return NodeType::NumberExpr; }
    };

    class CharExpr : public Expr {
    public:
        char value;

        CharExpr(char value) : value(value) {}

        string debug(int indent = 0) const override {
            return string(indent * 2, ' ') + "CharExpr: '" + string(1, value) + "'\n";
        }

        NodeType getKind() const override { return NodeType::CharExpr; }
    };

    class StringExpr : public Expr {
    public:
        string value;

        StringExpr(string value) : value(value) {}

        string debug(int indent = 0) const override {
            return string(indent * 2, ' ') + "StringExpr: \"" + value + "\"\n";
        }

        NodeType getKind() const override { return NodeType::StringExpr; }
    };

    class IdentExpr : public Expr {
    public:
        string value;

        IdentExpr(string value) : value(value) {}

        string debug(int indent = 0) const override {
            return string(indent * 2, ' ') + "IdentExpr: " + value + "\n";
        }

        NodeType getKind() const override { return NodeType::IdentExpr; }
    };

    class AssignmentExpr : public Expr {
    public:
        string identifier;
        ExprPtr value;

        AssignmentExpr(string identifier, ExprPtr value)
        : identifier(identifier), value(value) {}

        string debug(int indent = 0) const override {
            string result;
            result += string(indent * 2, ' ') + "AssignmentExpr: {\n";
            result += string((indent + 1) * 2, ' ') + "Identifier: " + identifier + "\n";
            result += string((indent + 1) * 2, ' ') + "Value: " + value->debug() + "\n";
            result += string(indent * 2, ' ') + "}\n";
            return result;
        }

        NodeType getKind() const override { return NodeType::AssignmentExpr; }
    };

    class BinaryExpr : public Expr {
    public:
        ExprPtr left;
        string op;
        ExprPtr right;

        BinaryExpr(ExprPtr left, string op, ExprPtr right)
            : left(left), op(op), right(right) {}

        string debug(int indent = 0) const override {
            string result;
            result += string(indent * 2, ' ') + "BinaryExpr: {\n";
            result += left->debug(indent + 1);
            result += string((indent + 1) * 2, ' ') + "Operator: " + op + "\n";
            result += right->debug(indent + 1);
            result += string(indent * 2, ' ') + "}\n";
            return result;
        }

        NodeType getKind() const override { return NodeType::BinaryExpr; }
    };

    class LogicalExpr : public Expr {
    public:
        ExprPtr left;
        string op;
        ExprPtr right;

        LogicalExpr(ExprPtr left, string op, ExprPtr right)
            : left(left), op(op), right(right) {}

        string debug(int indent = 0) const override {
            string result;
            result += string(indent * 2, ' ') + "LogicalExpr: {\n";
            result += left->debug(indent + 1);
            result += string((indent + 1) * 2, ' ') + "Operator: " + op + "\n";
            result += right->debug(indent + 1);
            result += string(indent * 2, ' ') + "}\n";
            return result;
        }

        NodeType getKind() const override { return NodeType::LogicalExpr; }
    };

    class ComparasonExpr : public Expr {
    public:
        ExprPtr left;
        string op;
        ExprPtr right;

        ComparasonExpr(ExprPtr left, string op, ExprPtr right)
            : left(left), op(op), right(right) {}

        string debug(int indent = 0) const override {
            string result;
            result += string(indent * 2, ' ') + "ComparasonExpr: {\n";
            result += left->debug(indent + 1);
            result += string((indent + 1) * 2, ' ') + "Operator: " + op + "\n";
            result += right->debug(indent + 1);
            result += string(indent * 2, ' ') + "}\n";
            return result;
        }

        NodeType getKind() const override { return NodeType::ComparasonExpr; }
    };

}
}
