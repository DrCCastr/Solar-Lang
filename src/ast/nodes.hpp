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

    enum class NodeType {
        BlockStmt,
        FuncStmt,
        ReturnStmt,

        NullExpr,
        BoolExpr,
        FloatExpr,
        IntExpr,
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

        TokenPos pos;
    };

    class Expr : public Stmt {
    public:
        virtual ~Expr() = default;

        Type type_;

        explicit Expr(Type type = Type()) : type_(type) {}
    };

    using StmtPtr = shared_ptr<Stmt>;
    using ExprPtr = shared_ptr<Expr>;

    // ----------|
    // Statments |
    // ----------|

    class BlockStmt : public Stmt {
    public:
        vector<StmtPtr> body;

        BlockStmt(TokenPos pos, vector<StmtPtr> body) : body(body) {
            this->pos = pos;
        }

        string debug(int indent = 0) const override {
            string result;
            result += string(indent * 2, ' ') + "BlockStmt: {\n";
            for (const auto& stmt : this->body) {
                result += stmt->debug(indent + 1);
            }
            result += string(indent * 2, ' ') + "}\n";
            return result;
        }

        NodeType getKind() const override { return NodeType::BlockStmt; }
    };

    class FuncStmt : public Stmt {
    public:
        string identifier;
        vector<StmtPtr> body;
        vector<pair<string, Type>> args;
        Type returnType;

        FuncStmt(TokenPos pos, string identifier, vector<StmtPtr> body, Type returnType) : identifier(identifier), body(body), returnType(returnType) {
            this->pos = pos;
        }

        string debug(int indent = 0) const override {
            string result;
            result += string(indent * 2, ' ') + "FuncStmt: {\n";

            result += string((indent + 1) * 2, ' ') + "Identifier: " + this->identifier + "\n";
            result += string((indent + 1) * 2, ' ') + "Return type: " + this->returnType.toString() + "\n";

            result += string((indent + 1) * 2, ' ') + "Args: {\n";
            for (const auto& arg : this->args) {
                result += string((indent + 2) * 2, ' ') + "Name: " + arg.first + "\n";
                result += string((indent + 2) * 2, ' ') + "Type: " + arg.second.toString() + "\n";
            }
            result += string((indent + 1) * 2, ' ') + "}\n";

            result += string((indent + 1) * 2, ' ') + "Body: {\n";
            for (const auto& stmt : this->body) {
                result += stmt->debug(indent + 2);
            }
            result += string((indent + 1) * 2, ' ') + "}\n";

            result += string(indent * 2, ' ') + "}\n";
            return result;
        }

        NodeType getKind() const override { return NodeType::FuncStmt; }
    };

    class ReturnStmt : public Stmt {
    public:
        ExprPtr ret;

        ReturnStmt(TokenPos pos, ExprPtr ret) : ret(ret) {
            this->pos = pos;
        }

        string debug(int indent = 0) const override {
            return string(indent * 2, ' ') + "ReturnStmt: " + this->ret->debug(indent);
        }

        NodeType getKind() const override { return NodeType::ReturnStmt; }
    };

    // ------------|
    // Expressions |
    // ------------|

    // Literal Expresisons //
    class NullExpr : public Expr {
    public:
        NullExpr(TokenPos pos) : Expr(Type(TypeEnum::Null)) {
            this->pos = pos;
        }

        string debug(int indent = 0) const override {
            return string(indent * 2, ' ') + "NullExpr\n";
        }

        NodeType getKind() const override { return NodeType::NullExpr; }
    };

    class BoolExpr : public Expr {
    public:
        bool value;

        BoolExpr(TokenPos pos, bool value) 
            : Expr(Type(TypeEnum::Bool)), value(value) {
            this->pos = pos;
        }

        string debug(int indent = 0) const override {
            return string(indent * 2, ' ') + "BoolExpr: " + to_string(this->value) + "\n";
        }

        NodeType getKind() const override { return NodeType::BoolExpr; }
    };

    class FloatExpr : public Expr {
    public:
        double value;

        FloatExpr(TokenPos pos, double value) 
            : Expr(Type(TypeEnum::Float)), value(value) {
            this->pos = pos;
        }

        string debug(int indent = 0) const override {
            return string(indent * 2, ' ') + "FloatExpr: " + to_string(this->value) + "\n";
        }

        NodeType getKind() const override { return NodeType::FloatExpr; }
    };

    class IntExpr : public Expr {
    public:
        int value;

        IntExpr(TokenPos pos, int value) 
            : Expr(Type(TypeEnum::Int)), value(value) {
            this->pos = pos;
        }

        string debug(int indent = 0) const override {
            return string(indent * 2, ' ') + "IntExpr: " + to_string(this->value) + "\n";
        }

        NodeType getKind() const override { return NodeType::IntExpr; }
    };

    class CharExpr : public Expr {
    public:
        char value;

        CharExpr(TokenPos pos, char value) 
            : Expr(Type(TypeEnum::Char)), value(value) {
            this->pos = pos;
        }

        string debug(int indent = 0) const override {
            return string(indent * 2, ' ') + "CharExpr: '" + string(1, this->value) + "'\n";
        }

        NodeType getKind() const override { return NodeType::CharExpr; }
    };

    class StringExpr : public Expr {
    public:
        string value;

        StringExpr(TokenPos pos, string value) 
            : Expr(Type(TypeEnum::String)), value(value) {
            this->pos = pos;
        }

        string debug(int indent = 0) const override {
            return string(indent * 2, ' ') + "StringExpr: \"" + this->value + "\"\n";
        }

        NodeType getKind() const override { return NodeType::StringExpr; }
    };

    class IdentExpr : public Expr {
    public:
        string value;

        IdentExpr(TokenPos pos, string value) 
            : Expr(Type(TypeEnum::Auto)), value(value) {
            this->pos = pos;
        }

        string debug(int indent = 0) const override {
            return string(indent * 2, ' ') + "IdentExpr: " + this->value + "\n";
        }

        NodeType getKind() const override { return NodeType::IdentExpr; }
    };

    // Complex expresisons //
    class AssignmentExpr : public Expr {
    public:
        string identifier;
        ExprPtr value;

        AssignmentExpr(TokenPos pos, string identifier, ExprPtr value)
        : Expr(Type(TypeEnum::Null)), identifier(identifier), value(value) {
            this->pos = pos;
        }

        string debug(int indent = 0) const override {
            string result;
            result += string(indent * 2, ' ') + "AssignmentExpr: {\n";
            result += string((indent + 1) * 2, ' ') + "Identifier: " + this->identifier + "\n";
            result += string((indent + 1) * 2, ' ') + "Value: " + this->value->debug() + "\n";
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

        BinaryExpr(TokenPos pos, ExprPtr left, string op, ExprPtr right)
            : left(left), op(op), right(right) {
                this->pos = pos;
            }

        string debug(int indent = 0) const override {
            string result;
            result += string(indent * 2, ' ') + "BinaryExpr: {\n";
            result += this->left->debug(indent + 1);
            result += string((indent + 1) * 2, ' ') + "Operator: " + this->op + "\n";
            result += this->right->debug(indent + 1);
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

        LogicalExpr(TokenPos pos, ExprPtr left, string op, ExprPtr right)
            : Expr(Type(TypeEnum::Bool)), left(left), op(op), right(right) {
                this->pos = pos;
            }

        string debug(int indent = 0) const override {
            string result;
            result += string(indent * 2, ' ') + "LogicalExpr: {\n";
            result += this->left->debug(indent + 1);
            result += string((indent + 1) * 2, ' ') + "Operator: " + this->op + "\n";
            result += this->right->debug(indent + 1);
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

        ComparasonExpr(TokenPos pos, ExprPtr left, string op, ExprPtr right)
            : Expr(Type(TypeEnum::Bool)), left(left), op(op), right(right) {
                this->pos = pos;
            }

        string debug(int indent = 0) const override {
            string result;
            result += string(indent * 2, ' ') + "ComparasonExpr: {\n";
            result += this->left->debug(indent + 1);
            result += string((indent + 1) * 2, ' ') + "Operator: " + this->op + "\n";
            result += this->right->debug(indent + 1);
            result += string(indent * 2, ' ') + "}\n";
            return result;
        }

        NodeType getKind() const override { return NodeType::ComparasonExpr; }
    };

}
