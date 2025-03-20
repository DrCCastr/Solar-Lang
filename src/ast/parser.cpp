/***
 * @file parser.cpp
 */

//////////////
// Includes //
//////////////

#include "parser.hpp"

using namespace std;

//////////
// Code //
//////////

namespace Solar {
namespace Ast {

    // Helpers //
    Lexer::Token Parser::next() {
        auto tk = this->actual();
        this->tokens.erase(this->tokens.begin());

        return tk;
    }
    Lexer::Token Parser::actual() {
        return this->tokens[0];
    }
    Lexer::Token Parser::expect(Lexer::TokenType expected) {
        auto tk = this->next();

        if (tk.type != expected) {
            this->errSession.addError(
                "Unexpected token, expecting" +
                Lexer::TToString(expected),
                tk.pos.file,
                tk.pos.line,
                tk.pos.collum
            );
        }

        return tk;
    }
    bool Parser::notEOF() {
        return this->tokens[0].type != Lexer::TokenType::EOF_;
    }

    // Initializers //
    shared_ptr<BlockStmt> Parser::parseCode(string source, string file) {
        this->tokens = Lexer::tokenize(source, file);
        vector<shared_ptr<Stmt>> body;

        while (this->notEOF()) {
            body.push_back(this->parseStmt());
        }

        this->errSession.debug();
        return make_shared<BlockStmt>(body);
    }

    // Statments //
    StmtPtr Parser::parseStmt() {
        return this->parseExpr();
    }

    // Expressions order
    //  Operators Expr
    //      Additive Expr
    //      Multiplicative Expr
    //  Primary Expr

    // Expresisons //
    template <typename expr>
    ExprPtr Parser::parseOperatorExpr(std::function<ExprPtr()> subExpr, const vector<string>& ops) {
        auto left = subExpr();

        auto validOp = [ops](string op) {
            for (auto const& inOp : ops)
                if (inOp == op) return true;

            return false;
        };

        while (validOp(this->actual().content)) {
            auto op = this->next();
            auto right = subExpr();

            return make_shared<expr>(left, op.content, right);
        }

        return left;
    }

    ExprPtr Parser::parseExpr() {
        return this->parseAdditiveExpr();
    }

    ExprPtr Parser::parseAdditiveExpr() {
        return this->parseOperatorExpr<BinaryExpr>([this](){ return this->parseMultiplicativeExpr(); }, {"+", "-"});
    }

    ExprPtr Parser::parseMultiplicativeExpr() {
        return this->parseOperatorExpr<BinaryExpr>([this](){ return this->parsePrimaryExpr(); }, {"*", "/"});
    }


    ExprPtr Parser::parsePrimaryExpr() {
        auto tk = this->actual().type;

        switch (tk) {
            case Lexer::TokenType::Number: return make_shared<NumberExpr>(stod(this->next().content)); break;
            case Lexer::TokenType::Char: return make_shared<CharExpr>(this->next().content[0]); break;
            case Lexer::TokenType::String: return make_shared<StringExpr>(this->next().content); break;
            case Lexer::TokenType::Identfier: return make_shared<IdentExpr>(this->next().content); break;

            case Lexer::TokenType::OpenParen: {
                this->next();
                auto tk = this->parseExpr();
                this->expect(Lexer::TokenType::CloseParen);

                return tk;
                break;
            }

            default: {
                auto tok = this->next();
                this->errSession.addError(
                    "Unknow token: " +
                    Lexer::TToString(tk),
                    tok.pos.file,
                    tok.pos.line,
                    tok.pos.collum
                );

                return nullptr;
                break;
            }
        }
    }

}
}
