/***
 * @file nodes.hpp
 */

#pragma once

//////////////
// Includes //
//////////////

#include "solar_pack.hpp"
#include <string>
#include <variant>

using namespace std;

//////////
// Code //
//////////

namespace Solar {
namespace Ast {

    enum class NodeType {
        Block,
    };

    using StmtT = variant<ExprStmt, BlockStmt>;
    using ExprV = variant<>;

    struct Stmt {};
    struct Expr : Stmt {};

    // ----------|
    // Statments |
    // ----------|

    struct ExprStmt : Stmt {
        Expr expr;
    };

    struct BlockStmt : Stmt {
        vector<Lexer::Token> body;
    };

    // ------------|
    // Expressions |
    // ------------|

    struct NumberExpr : Expr {
        double value;
    };

    struct CharExpr : Expr {
        char value;
    };

    struct StringExpr : Expr {
        string value;
    };

    struct IdentExpr : Expr {
        string value;
    };

}
}