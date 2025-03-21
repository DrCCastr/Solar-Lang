/***
 * @file parser.hpp
 */

#pragma once

//////////////
// Includes //
//////////////

#include "solar_pack.hpp"
#include <vector>
#include <functional>
#include <unordered_set>
#include <memory>

using namespace std;

//////////
// Code //
//////////

namespace Solar {
namespace Ast {

    class Parser {
    private:
        Error::ErrorSesion errSession;
        vector<Lexer::Token> tokens;

        // Helpers //
        Lexer::Token next();
        Lexer::Token actual();
        Lexer::Token expect(Lexer::TokenType expeted);
        bool notEOF();

        // Statments //
        StmtPtr parseStmt();

        // Expressions //
        template <typename Expr>
        ExprPtr parseOperatorExpr(std::function<ExprPtr()> subExpr, const vector<string>& ops);
        ExprPtr parseExpr();
        ExprPtr parseAssignExpr();
        ExprPtr parseLogicalExpr();
        ExprPtr parseComparasonExpr();
        ExprPtr parseAdditiveExpr();
        ExprPtr parseMultiplicativeExpr();

        ExprPtr parsePrimaryExpr();
    public:
        // Intializers //
        shared_ptr<BlockStmt> parseCode(string source, string file = "Unknow");
    };

}
}
