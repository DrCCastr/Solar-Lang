/***
 * @file compiler.cpp
 */

//////////////
// Includes //
//////////////

#include "compiler.hpp"

using namespace std;

//////////
// Code //
//////////

namespace Solar {

    // Initializers //
    void Compiler::compileCode(string code) {
        Solar::Parser parser;
        auto block = parser.parseCode(code);
        cout << block->debug();
        this->visitBlock(block);

        std::error_code EC;
        llvm::raw_fd_ostream outFile("../output.ll", EC);
        this->module->print(llvm::errs(), nullptr);
        this->module->print(outFile, nullptr);
    }

    // Statments //
    void Compiler::compile(StmtPtr node) {
        switch (node->getKind()) {
            case NodeType::BlockStmt:
                this->visitBlock(static_pointer_cast<BlockStmt>(node));
                break;
            case NodeType::FuncStmt:
                this->visitFunc(static_pointer_cast<FuncStmt>(node));
                break;
            case NodeType::ReturnStmt:
                this->visitReturn(static_pointer_cast<ReturnStmt>(node));
                break;
            case NodeType::VarDecStmt:
                this->visitVarDecl(static_pointer_cast<VarDecStmt>(node));
                break;

            default:
                this->visitExpr(static_pointer_cast<Expr>(node));
                break;
        }
    }

    void Compiler::visitBlock(shared_ptr<BlockStmt> node) {
        for (StmtPtr stmt : node->body) {
            this->compile(stmt);
        }
    }

    void Compiler::visitFunc(shared_ptr<FuncStmt> node) {
        auto returnType = this->typeMap[node->returnType.kind];

        vector<llvm::Type*> argTypes;
        for (const auto& param : node->args) {
            argTypes.push_back(this->typeMap[param.second.kind]);
        }

        auto funcType = llvm::FunctionType::get(returnType, argTypes, false);
        auto func = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, node->identifier, this->module);

        auto entryBlock = llvm::BasicBlock::Create(this->context, "entry", func);
        this->builder.SetInsertPoint(entryBlock);

        auto argsIt = func->arg_begin();
        for (auto& [name, type] : node->args) {
            argsIt->setName(name);
            this->namedValues[name] = argsIt;
            ++argsIt;
        }
        

        for (const auto& stmt : node->body) {
            this->compile(stmt);
        }

        if (returnType->isVoidTy()) {
            this->builder.CreateRetVoid();
        } else {
            if (!this->builder.GetInsertBlock()->getTerminator()) {
                llvm::Value* defaultRet = llvm::Constant::getNullValue(returnType);
                this->builder.CreateRet(defaultRet);
            }
        }
    }

    void Compiler::visitReturn(shared_ptr<ReturnStmt> node) {
        auto value = this->visitExpr(node->ret);

        if (auto identNode = dynamic_cast<IdentExpr*>(node->ret.get())) {
            auto varIt = this->namedValues.find(identNode->value);
            if (varIt != this->namedValues.end()) {
                llvm::Value* var = varIt->second;

                if (llvm::AllocaInst* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(var)) {
                    value = this->builder.CreateLoad(allocaInst->getAllocatedType(), var, identNode->value);
                }
            }
        }

        this->builder.CreateRet(value);
    }

    void Compiler::visitVarDecl(shared_ptr<VarDecStmt> node) {
        auto type = this->typeMap[node->value->type_.kind];

        if (node->value->type_.isPointer) {
            auto value = this->visitExpr(node->value);
            this->namedValues[node->identifier] = value;
        } else {
            auto var = this->builder.CreateAlloca(type, nullptr, node->identifier);
            this->namedValues[node->identifier] = var;

            if (node->value != nullptr) {
                auto value = this->visitExpr(node->value);
                this->builder.CreateStore(value, var);
            }
        }
    }

    // Expressions //
    llvm::Value* Compiler::visitExpr(ExprPtr node) {
        switch (node->getKind()) {
            case NodeType::AssignmentExpr:
                return this->visitAssignExpr(static_pointer_cast<AssignmentExpr>(node));
                break;
            case NodeType::LogicalExpr:
                return this->visitLogicalExpr(static_pointer_cast<LogicalExpr>(node));
                break;
            case NodeType::ComparasonExpr:
                return this->visitCompareExpr(static_pointer_cast<ComparasonExpr>(node));
                break;
            case NodeType::BinaryExpr:
                return this->visitBinaryExpr(static_pointer_cast<BinaryExpr>(node));
                break;
            case NodeType::UnaryExpr:
                return this->visitUnaryExpr(static_pointer_cast<UnaryExpr>(node));
                break;
            case NodeType::CallExpr:
                return this->visitCallExpr(static_pointer_cast<CallExpr>(node));
                break;

            default:
                return this->visitPrimaryExpr(node);
                break;
        }
    }

    llvm::Value* Compiler::visitAssignExpr(shared_ptr<AssignmentExpr> node) {
        auto value = this->visitExpr(node->value);
        auto var = this->namedValues[node->identifier];

        return this->builder.CreateStore(value, var);
    }

    llvm::Value* Compiler::visitCallExpr(shared_ptr<CallExpr> node) {
        llvm::FunctionCallee func;

        if (node->isExpr) {
            cout << "I dont implement this" << endl;
        } else {
            func = this->module->getFunction(static_pointer_cast<IdentExpr>(node->left)->value);
        }

        vector<llvm::Value*> args;
        for (const auto& arg : node->args) {
            args.push_back(this->visitExpr(arg));
        }

        return this->builder.CreateCall(func, args, "calltmp");
    }

    llvm::Value* Compiler::visitLogicalExpr(shared_ptr<LogicalExpr> node) {
        auto left = this->visitExpr(node->left);
        auto right = this->visitExpr(node->right);

        if (node->op == "&&") {
            return this->builder.CreateAnd(left, right, "andtmp");
        } else if (node->op == "||") {
            return this->builder.CreateOr(left, right, "ortmp");
        } else {
            return llvm::Constant::getNullValue(llvm::Type::getVoidTy(this->context));
        }
    }

    llvm::Value* Compiler::visitCompareExpr(shared_ptr<ComparasonExpr> node) {
        auto left = this->visitExpr(node->left);
        auto right = this->visitExpr(node->right);

        if (node->op == "==") {
            return this->builder.CreateICmpEQ(left, right, "eqtmp");
        } else if (node->op == "!=") {
            return this->builder.CreateICmpNE(left, right, "netmp");
        } else if (node->op == ">") {
            return this->builder.CreateICmpSGT(left, right, "gttmp");
        } else if (node->op == "<") {
            return this->builder.CreateICmpSLT(left, right, "lttmp");
        } else if (node->op == ">=") {
            return this->builder.CreateICmpSGE(left, right, "getmp");
        } else if (node->op == "<=") {
            return this->builder.CreateICmpSLE(left, right, "letmp");
        } else {
            return llvm::Constant::getNullValue(llvm::Type::getVoidTy(this->context));
        }
    }

    llvm::Value* Compiler::visitBinaryExpr(shared_ptr<BinaryExpr> node) {
        auto left = this->visitExpr(node->left);
        auto right = this->visitExpr(node->right);

        if (node->op == "+") {
            return this->builder.CreateAdd(left, right, "addtmp");
        } else if (node->op == "-") {
            return this->builder.CreateSub(left, right, "subtmp");
        } else if (node->op == "*") {
            return this->builder.CreateMul(left, right, "multmp");
        } else if (node->op == "/") {
            return this->builder.CreateSDiv(left, right, "divtmp");
        } else if (node->op == "%") {
            return this->builder.CreateSRem(left, right, "modtmp");
        } else if (node->op == "^") {
            llvm::Function *powFunction = llvm::Intrinsic::getDeclaration(
                this->module, llvm::Intrinsic::pow, {llvm::Type::getDoubleTy(this->context), llvm::Type::getDoubleTy(this->context)}
            );
            return this->builder.CreateCall(powFunction, {left, right}, "powtmp");
        } else {
            return llvm::Constant::getNullValue(llvm::Type::getVoidTy(this->context));
        }
    }

    llvm::Value* Compiler::visitUnaryExpr(shared_ptr<UnaryExpr> node) {
        auto expr = this->visitExpr(node->value);

        if (node->op == "-") {
            return this->builder.CreateNeg(expr, "negtmp");
        } else if (node->op == "!") {
            return this->builder.CreateNot(expr, "nottmp");
        } else {
            return llvm::Constant::getNullValue(llvm::Type::getVoidTy(this->context));
        }
    }

    llvm::Value* Compiler::visitPrimaryExpr(ExprPtr node) {
        switch (node->getKind()) {
            case NodeType::NullExpr: {
                return llvm::Constant::getNullValue(llvm::Type::getVoidTy(this->context));
            }
            case NodeType::BoolExpr: {
                bool value = static_cast<BoolExpr*>(node.get())->value;
                return llvm::ConstantInt::get(llvm::Type::getInt1Ty(this->context), value);
            }
            case NodeType::IntExpr: {
                int value = static_cast<IntExpr*>(node.get())->value;
                return llvm::ConstantInt::get(llvm::Type::getInt32Ty(this->context), value);
            }
            case NodeType::DoubleExpr: {
                double value = static_cast<DoubleExpr*>(node.get())->value;
                return llvm::ConstantFP::get(llvm::Type::getDoubleTy(this->context), value);
            }
            case NodeType::FloatExpr: {
                double value = static_cast<FloatExpr*>(node.get())->value;
                return llvm::ConstantFP::get(llvm::Type::getFloatTy(this->context), value);
            }
            case NodeType::CharExpr: {
                char value = static_cast<CharExpr*>(node.get())->value;
                return llvm::ConstantInt::get(llvm::Type::getInt8Ty(this->context), value);
            }
            case NodeType::IdentExpr: {
                auto identNode = static_pointer_cast<IdentExpr>(node);
                auto varIt = this->namedValues.find(identNode->value);

                llvm::Value* var = varIt->second;

                if (llvm::AllocaInst* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(var)) {
                    return this->builder.CreateLoad(allocaInst->getAllocatedType(), var, identNode->value);
                }

                return var;
            }
            default: {
                return llvm::Constant::getNullValue(llvm::Type::getVoidTy(this->context));
            }
        }
    }

}