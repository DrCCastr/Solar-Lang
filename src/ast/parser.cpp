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
    // Helpers //
    Token Parser::next() {
        auto tk = this->actual();
        this->tokens.erase(this->tokens.begin());

        return tk;
    }

    Token Parser::actual() {
        return this->tokens[0];
    }

    Token Parser::peek() {
        return this->tokens[1];
    }

    Token Parser::expect(TokenType expected) {
        auto tk = this->next();

        if (tk.type != expected) {
            this->errSession.addError(
                "Unexpected token, expecting " +
                TToString(expected) +
                ", but founded: " +
                TToString(tk.type),
                tk.pos.file,
                tk.pos.line,
                tk.pos.collum
            );
        }

        return tk;
    }

    Type Parser::parseType() {
        auto tk = this->next();

        if (tk.type != TokenType::Identfier && tk.type != TokenType::Null) {
            this->errSession.addError(
                "Expected type, but founded: " +
                TToString(tk.type),
                tk.pos.file,
                tk.pos.line,
                tk.pos.collum
            );

            return Type();
        }

        auto itPrimary = primaryTypes.find(tk.content);
        if (itPrimary != primaryTypes.end()) {
            return Type(itPrimary->second);
        }

        auto itUserType = this->userTypes.find(tk.content);
        if (itUserType != this->userTypes.end()) {
            return itUserType->second;
        }

        this->errSession.addError(
            "Expecting type name, but founded: " +
            tk.content,
            tk.pos.file,
            tk.pos.line,
            tk.pos.collum
        );
        return Type(); // Implement
    }

    bool Parser::notEOF() {
        return this->tokens[0].type != TokenType::EOF_;
    }

    // Initializers //
    shared_ptr<BlockStmt> Parser::parseCode(string source, string file) {
        this->tokens = tokenize(source, file);
        vector<shared_ptr<Stmt>> body;

        while (this->notEOF()) {
            body.push_back(this->parseStmt());
        }

        this->errSession.debug();
        return make_shared<BlockStmt>(body[0]->pos, body);
    }

    // Statments //
    StmtPtr Parser::parseStmt() {
        switch (this->actual().type) {
            case TokenType::Func: {
                return this->parseFuncStmt();
                break;
            }

            default: return this->parseExpr(); break;
        }
    }

    StmtPtr Parser::parseFuncStmt(string name) {
        this->next();
        vector<StmtPtr> body;
        vector<pair<string, Type>> args;

        if (name.empty()) {
            name = this->expect(TokenType::Identfier).content;
        }

        this->expect(TokenType::OpenParen);

        while (this->notEOF() && this->actual().type != TokenType::CloseParen) {
            auto ident = this->expect(TokenType::Identfier).content;
            this->expect(TokenType::Colon);
            auto type_ = this->parseType();

            args.push_back({ident, type_});

            if (this->actual().type == TokenType::Comma) {
                this->next();
            } else {
                break;
            }
        }

        this->next();

        bool autoType = false;
        Type returnType;
        if (this->actual().type == TokenType::Colon) {
            this->next();
            returnType = this->parseType();
        } else {
            autoType = true;
        }

        this->expect(TokenType::OpenCurly);

        while (this->notEOF() && this->actual().type != TokenType::CloseCurly) {

            switch (this->actual().type) {
                case TokenType::Return: {
                    this->next();
                    auto ret = this->parseExpr();

                    if (autoType) {
                        autoType = false;
                        returnType = ret->type_;
                    } else if (!ret->type_.canConvert(returnType)) {
                        this->errSession.addError(
                            "Expected return type: " +
                            returnType.toString() +
                            ", but found: " +
                            ret->type_.toString(),
                            ret->pos.file,
                            ret->pos.line,
                            ret->pos.collum
                        );
                    }

                    body.push_back(make_shared<ReturnStmt>(ret->pos, ret));
                    break;
                }
                default:
                    body.push_back(this->parseStmt());
                    break;
            }
        }

        this->expect(TokenType::CloseCurly);

        return make_shared<FuncStmt>(body[0]->pos, name, body, returnType);
    }

    // Expressions order
    // Assignment Expr
    // Logical Expr
    // Comparason Expr
    // Additive Expr
    // Multiplicative Expr
    // Primary Expr

    // Expresisons //
    template <typename expr>
    ExprPtr Parser::parseOperatorExpr(std::function<ExprPtr()> subExpr, const vector<string>& ops, bool typeCheck) {
        auto left = subExpr();

        auto validOp = [ops](string op) {
            for (auto const& inOp : ops)
                if (inOp == op) return true;

            return false;
        };

        while (this->notEOF() && validOp(this->actual().content)) {
            auto op = this->next();
            auto right = subExpr();

            if (typeCheck && !right->type_.canConvert(left->type_)) {
                this->errSession.addError(
                    "Expected a value of type: " +
                    left->type_.toString() +
                    ", but found: " +
                    right->type_.toString(),
                    op.pos.file,
                    op.pos.line,
                    op.pos.collum
                );
            }

            left = make_shared<expr>(op.pos, left, op.content, right);

            if (typeCheck) left->type_ = right->type_; 
        }

        return left;
    }

    ExprPtr Parser::parseExpr() {
        return this->parseAssignExpr();
    }

    ExprPtr Parser::parseAssignExpr() {
        auto left = this->parseLogicalExpr();

        if (left->getKind() == NodeType::IdentExpr && this->actual().content == "=") {
            auto op = this->next();
            auto right = this->parseLogicalExpr();

            left = make_shared<AssignmentExpr>(op.pos, static_cast<IdentExpr*>(left.get())->value, right);
        }

        return left;
    }

    ExprPtr Parser::parseLogicalExpr() {
        return this->parseOperatorExpr<LogicalExpr>([this](){ return this->parseComparasonExpr(); }, {"&&", "||"});
    }

    ExprPtr Parser::parseComparasonExpr() {
        return this->parseOperatorExpr<ComparasonExpr>([this](){ return this->parseAdditiveExpr(); }, {"<=", "<", ">", ">=", "==", "!="});
    }

    ExprPtr Parser::parseAdditiveExpr() {
        return this->parseOperatorExpr<BinaryExpr>([this](){ return this->parseMultiplicativeExpr(); }, {"+", "-"}, true);
    }

    ExprPtr Parser::parseMultiplicativeExpr() {
        return this->parseOperatorExpr<BinaryExpr>([this](){ return this->parsePrimaryExpr(); }, {"*", "/"}, true);
    }

    ExprPtr Parser::parsePrimaryExpr() {
        auto tk = this->actual();

        switch (tk.type) {
            case TokenType::Null: return make_shared<NullExpr>(tk.pos); break;
            case TokenType::Bool: return make_shared<BoolExpr>(tk.pos, this->next().content == "true" ? true : false); break;
            case TokenType::Int: return make_shared<IntExpr>(tk.pos, stoi(this->next().content)); break;
            case TokenType::Float: return make_shared<FloatExpr>(tk.pos, stod(this->next().content)); break;
            case TokenType::Char: return make_shared<CharExpr>(tk.pos, this->next().content[0]); break;
            case TokenType::String: return make_shared<StringExpr>(tk.pos, this->next().content); break;
            case TokenType::Identfier: return make_shared<IdentExpr>(tk.pos, this->next().content); break;

            case TokenType::OpenParen: {
                this->next();
                auto tkn = this->parseExpr();
                this->expect(TokenType::CloseParen);

                return tkn;
                break;
            }

            default: {
                auto tok = this->next();
                this->errSession.addError(
                    "Unexpected token: " +
                    TToString(tk.type),
                    tok.pos.file,
                    tok.pos.line,
                    tok.pos.collum
                );

                return make_shared<NullExpr>(TokenPos());
                break;
            }
        }
    }

}
