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

    Token Parser::opcional(TokenType expected) {
        auto tk = this->actual();

        if (tk.type == expected) this->next();

        return tk;
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

    Type Parser::parseType(AstEnv& env) {
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

        if (env.hasUserType(tk.content)) {
            return env.getUserType(tk.content);
        }

        this->errSession.addError(
            "Expecting type name, but founded: " +
            tk.content,
            tk.pos.file,
            tk.pos.line,
            tk.pos.collum
        );
        return Type();
    }

    bool Parser::notEOF() {
        return this->tokens[0].type != TokenType::EOF_;
    }

    // Initializers //
    shared_ptr<BlockStmt> Parser::parseCode(string source, string file) {
        this->tokens = tokenize(source, file);
        vector<shared_ptr<Stmt>> body;
        AstEnv env;

        while (this->notEOF()) {
            body.push_back(this->parseStmt(env));
        }

        this->errSession.debug();
        return make_shared<BlockStmt>(body[0]->pos, body);
    }

    // Statments //
    StmtPtr Parser::parseStmt(AstEnv& env) {
        switch (this->actual().type) {
            case TokenType::Func: {
                return this->parseFuncStmt("", env);
                break;
            }
            case TokenType::Return: {
                return this->parseReturnStmt(env);
                break;
            }
            case TokenType::Var: {
                return this->parseVarDecStmt(env);
                break;
            }

            default: return this->parseExpr(env); break;
        }
    }

    StmtPtr Parser::parseFuncStmt(string name, AstEnv& env) {
        this->next();
        vector<StmtPtr> body;
        unordered_map<string, Type> args;
        vector<Type> argsType;

        AstEnv newEnv(env);

        if (name.empty()) {
            name = this->expect(TokenType::Identfier).content;
        }

        this->expect(TokenType::OpenParen);

        while (this->notEOF() && this->actual().type != TokenType::CloseParen) {
            auto ident = this->expect(TokenType::Identfier).content;
            this->expect(TokenType::Colon);
            auto type_ = this->parseType(newEnv);

            args[ident] = type_;
            argsType.push_back(type_);
            newEnv.addParameter(ident, type_);

            if (this->actual().type == TokenType::Comma) {
                this->next();
            } else {
                break;
            }
        }

        this->next();

        if (this->actual().type == TokenType::Colon) {
            this->next();
            newEnv.returnType = this->parseType(env);
            newEnv.autoRetType = false;
        }

        this->expect(TokenType::OpenCurly);

        while (this->notEOF() && this->actual().type != TokenType::CloseCurly) {
            body.push_back(this->parseStmt(newEnv));
        }

        this->expect(TokenType::CloseCurly);

        env.addFunction(name, newEnv.returnType, argsType);
        return make_shared<FuncStmt>(body[0]->pos, name, body, args, newEnv.returnType);
    }

    StmtPtr Parser::parseReturnStmt(AstEnv& env) {
        auto tk = this->next();
        auto expr = this->parseExpr(env);

        if (env.autoRetType) {
            env.returnType = expr->type_;
        }

        if (!expr->type_.compare(env.returnType)) {
            this->errSession.addError(
                "Expected a return value of type: " +
                env.returnType.toString() +
                ", but found: " +
                expr->type_.toString(),
                tk.pos.file,
                tk.pos.line,
                tk.pos.collum
            );
        }

        this->opcional(TokenType::Semicolon);
        return make_shared<ReturnStmt>(tk.pos, expr);
    }

    StmtPtr Parser::parseVarDecStmt(AstEnv& env) {
        auto tk = this->next();
        auto ident = this->expect(TokenType::Identfier).content;

        if (env.hasVariable(ident)) {
            this->errSession.addError(
                "Variable: " + ident + " already declared",
                tk.pos.file, tk.pos.line, tk.pos.collum
            );
        }

        bool autoType = false;
        Type type;

        if (this->actual().type == TokenType::Colon) {
            this->next();
            type = this->parseType(env);
        } else {
            autoType = true;
        }

        ExprPtr expr = nullptr;
        if (this->actual().type == TokenType::Assignment) {
            this->next();
            expr = this->parseExpr(env);
        }

        if (autoType) {
            if (!expr) {
                this->errSession.addError(
                    "Cannot infer type for variable: " + ident + " without an assignment",
                    tk.pos.file, tk.pos.line, tk.pos.collum
                );
            } else {
                type = expr->type_;
            }
        }

        if (!autoType && expr && !expr->type_.compare(type)) {
            this->errSession.addError(
                "Expected a value of type: " + type.toString() + 
                ", but found: " + expr->type_.toString(),
                tk.pos.file, tk.pos.line, tk.pos.collum
            );
        }

        env.addVariable(ident, type);
        this->opcional(TokenType::Semicolon);

        return make_shared<VarDecStmt>(tk.pos, ident, expr);
    }

    // Expressions order
    // Assignment Expr
    // Call Expr
    // Logical Expr
    // Comparason Expr
    // Additive Expr
    // Multiplicative Expr
    // Exponential Expr
    // Unary Expr
    // Primary Expr

    // Expresisons //
    template <typename expr>
    ExprPtr Parser::parseOperatorExpr(std::function<ExprPtr()> subExpr, const vector<string>& ops, bool typeCheck, [[maybe_unused]] AstEnv& env) {
        auto left = subExpr();

        auto validOp = [ops](string op) {
            for (auto const& inOp : ops)
                if (inOp == op) return true;

            return false;
        };

        while (this->notEOF() && validOp(this->actual().content)) {
            auto op = this->next();
            auto right = subExpr();

            if (typeCheck && !right->type_.compare(left->type_)) {
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

    ExprPtr Parser::parseExpr(AstEnv& env) {
        return this->parseAssignExpr(env);
    }

    ExprPtr Parser::parseAssignExpr(AstEnv& env) {
        auto left = this->parseCallExpr(env);

        if (left->getKind() == NodeType::IdentExpr && this->actual().content == "=") {
            auto op = this->next();
            auto right = this->parseCallExpr(env);

            left = make_shared<AssignmentExpr>(op.pos, static_pointer_cast<IdentExpr>(left)->value, right);
        }

        return left;
    }

    ExprPtr Parser::parseCallExpr(AstEnv& env) {
        auto left = this->parseLogicalExpr(env);

        while (this->actual().type == TokenType::OpenParen) {

            if (!left || left->type_.kind != TypeEnum::Function) {
                this->errSession.addError(
                    "Cannot call a non-function value or null expression",
                    this->actual().pos.file,
                    this->actual().pos.line,
                    this->actual().pos.collum
                );
                this->next();
                break;
            }

            auto op = this->expect(TokenType::OpenParen);
            vector<ExprPtr> args;
            vector<Type> argsType;

            while (this->notEOF() && this->actual().type != TokenType::CloseParen) {
                auto arg = this->parseExpr(env);
                args.push_back(arg);
                argsType.push_back(arg->type_);

                if (this->actual().type == TokenType::Comma) {
                    this->next();
                } else {
                    break;
                }
            }

            for (size_t i = 0; i < argsType.size(); ++i) {
                if (i >= left->type_.unsizedGenerics[0].size() || 
                    !argsType[i].compare(left->type_.unsizedGenerics[0][i])) {
                    this->errSession.addError(
                        "Expected argument " + to_string(i + 1) + " of type: " +
                        (i < left->type_.unsizedGenerics[0].size() ? 
                            left->type_.unsizedGenerics[0][i].toString() : "unknown") +
                        ", but found: " +
                        argsType[i].toString(),
                        op.pos.file,
                        op.pos.line,
                        op.pos.collum
                    );
                }
            }

            this->expect(TokenType::CloseParen);
            left = make_shared<CallExpr>(op.pos, left->type_.generics[0].kind, left, args);
        }

        return left;
    }

    ExprPtr Parser::parseLogicalExpr(AstEnv& env) {
        return this->parseOperatorExpr<LogicalExpr>([this, &env](){ return this->parseComparasonExpr(env); }, {"&&", "||"}, false, env);
    }

    ExprPtr Parser::parseComparasonExpr(AstEnv& env) {
        return this->parseOperatorExpr<ComparasonExpr>([this, &env](){ return this->parseAdditiveExpr(env); }, {"<=", "<", ">", ">=", "==", "!="}, false, env);
    }

    ExprPtr Parser::parseAdditiveExpr(AstEnv& env) {
        return this->parseOperatorExpr<BinaryExpr>([this, &env](){ return this->parseMultiplicativeExpr(env); }, {"+", "-"}, true, env);
    }

    ExprPtr Parser::parseMultiplicativeExpr(AstEnv& env) {
        return this->parseOperatorExpr<BinaryExpr>([this, &env](){ return this->parseExponentialExpr(env); }, {"*", "/", "%"}, true, env);
    }

    ExprPtr Parser::parseExponentialExpr(AstEnv& env) {
        return this->parseOperatorExpr<BinaryExpr>([this, &env](){ return this->parseUnaryExpr(env); }, {"^"}, true, env);
    }

    ExprPtr Parser::parseUnaryExpr(AstEnv& env) {
        auto tk = this->actual();

        if (tk.content == "-" || tk.content == "!") {
            auto op = this->next();
            auto right = this->parseUnaryExpr(env);

            return make_shared<UnaryExpr>(op.pos, op.content, right);
        }

        return this->parsePrimaryExpr(env);
    }

    ExprPtr Parser::parsePrimaryExpr(AstEnv& env) {
        auto tk = this->actual();

        switch (tk.type) {
            case TokenType::Null: return make_shared<NullExpr>(tk.pos); break;
            case TokenType::Bool: return make_shared<BoolExpr>(tk.pos, this->next().content == "true" ? true : false); break;
            case TokenType::Int: return make_shared<IntExpr>(tk.pos, stoi(this->next().content)); break;
            case TokenType::Double: return make_shared<DoubleExpr>(tk.pos, stold(this->next().content)); break;
            case TokenType::Float: return make_shared<FloatExpr>(tk.pos, stod(this->next().content)); break;
            case TokenType::Char: return make_shared<CharExpr>(tk.pos, this->next().content[0]); break;

            case TokenType::Identfier: {
                if (!env.hasValue(tk.content)) {
                    this->errSession.addError(
                        "Used variable: " +
                        tk.content +
                        " not declared",
                        tk.pos.file,
                        tk.pos.line,
                        tk.pos.collum
                    );
                }

                return make_shared<IdentExpr>(tk.pos, env.getValueType(tk.content), this->next().content); 
                break;
            }

            case TokenType::OpenParen: {
                this->next();
                auto tkn = this->parseExpr(env);
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
