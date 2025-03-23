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
#include <unordered_map>
#include <memory>

using namespace std;

//////////
// Code //
//////////

namespace Solar {

    class Parser {
    private:
        ErrorSesion errSession;
        vector<Token> tokens;
        unordered_map<string, Type> userTypes;

        // Helpers //
        Token next();
        Token actual();
        Token peek(); // Next token without eating
        Token expect(TokenType expeted);
        Type parseType();
        bool notEOF();

        // Statments //
        StmtPtr parseStmt();
        StmtPtr parseFuncStmt(string name = "");

        // Expressions //
        template <typename Expr>
        ExprPtr parseOperatorExpr(std::function<ExprPtr()> subExpr, const vector<string>& ops, bool typeCheck = false);
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
