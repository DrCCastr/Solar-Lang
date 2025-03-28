/***
 * @file parser.hpp
 */

#pragma once

//////////////
// Includes //
//////////////

#include "lexer/pack.hpp"
#include "pack.hpp"
#include "error.hpp"
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

        // Helpers //
        Token next();
        Token actual();
        Token opcional(TokenType expected);
        Token expect(TokenType expeted);
        Type parseType(AstEnv& env);
        bool notEOF();

        // Statments //
        StmtPtr parseStmt(AstEnv& env);
        StmtPtr parseFuncStmt(string name, AstEnv& env);
        StmtPtr parseReturnStmt(AstEnv& env);
        StmtPtr parseVarDecStmt(AstEnv& env);

        // Expressions //
        template <typename Expr>
        ExprPtr parseOperatorExpr(std::function<ExprPtr()> subExpr, const vector<string>& ops, bool typeCheck, AstEnv& env);
        ExprPtr parseExpr(AstEnv& env);
        ExprPtr parseAssignExpr(AstEnv& env);
        ExprPtr parseLogicalExpr(AstEnv& env);
        ExprPtr parseComparasonExpr(AstEnv& env);
        ExprPtr parseAdditiveExpr(AstEnv& env);
        ExprPtr parseMultiplicativeExpr(AstEnv& env);
        ExprPtr parseExponentialExpr(AstEnv& env);
        ExprPtr parseUnaryExpr(AstEnv& env);
        ExprPtr parseCallExpr(AstEnv& env);

        ExprPtr parsePrimaryExpr(AstEnv& env);
    public:
        // Intializers //
        shared_ptr<BlockStmt> parseCode(string source, string file = "Unknow");
    };

}
