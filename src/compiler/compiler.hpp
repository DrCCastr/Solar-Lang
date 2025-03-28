/***
 * @file compiler.hpp
 */

//////////////
// Includes //
//////////////

#pragma once

#include "ast/pack.hpp"
#include <unordered_map>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Support/raw_ostream.h>

using namespace std;

//////////
// Code //
//////////

namespace Solar {

    class Compiler {
    private:
        llvm::LLVMContext context;
        llvm::Module* module;
        llvm::IRBuilder<> builder;
        unordered_map<TypeEnum, llvm::Type*> typeMap;
        unordered_map<string, llvm::Value*> namedValues;

        // Statments //
        void compile(StmtPtr node);
        void visitBlock(shared_ptr<BlockStmt> node);
        void visitFunc(shared_ptr<FuncStmt> node);
        void visitReturn(shared_ptr<ReturnStmt> node);
        void visitVarDecl(shared_ptr<VarDecStmt> node);

        // Expressions //
        llvm::Value* visitExpr(ExprPtr node);

        llvm::Value* visitAssignExpr(shared_ptr<AssignmentExpr> node);
        llvm::Value* visitCallExpr(shared_ptr<CallExpr> node);

        llvm::Value* visitLogicalExpr(shared_ptr<LogicalExpr> node);
        llvm::Value* visitCompareExpr(shared_ptr<ComparasonExpr> node);
        llvm::Value* visitBinaryExpr(shared_ptr<BinaryExpr> node);
        llvm::Value* visitUnaryExpr(shared_ptr<UnaryExpr> node);

        llvm::Value* visitPrimaryExpr(ExprPtr node);
    public:
        Compiler() : module(new llvm::Module("Main", context)), builder(llvm::IRBuilder<>(context)) {
            this->typeMap[TypeEnum::Null] = llvm::Type::getVoidTy(context);
            this->typeMap[TypeEnum::Bool] = llvm::Type::getInt1Ty(context);
            this->typeMap[TypeEnum::Int] = llvm::Type::getInt32Ty(context);
            this->typeMap[TypeEnum::Float] = llvm::Type::getFloatTy(context);
            this->typeMap[TypeEnum::Double] = llvm::Type::getDoubleTy(context);
            this->typeMap[TypeEnum::Char] = llvm::Type::getInt8Ty(context);
        }

        void compileCode(string code);
    };

}