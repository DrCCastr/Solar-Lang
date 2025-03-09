/***
 * @file ast_error.hpp
 * @brief Defines the AST error classes.
 */

#pragma once

#include "error_base.hpp"

/***
 * @namespace Error
 * @brief Implements Ast errors to namespace.
 */
namespace Error {
    class AstError : public BaseError {
    public:
        explicit AstError(const std::string& subType, const std::string& message) 
            : BaseError("( AST ) " + subType, message) {}
    };

    class SyntaxError : public AstError {
    public:
        explicit SyntaxError(const std::string& message)
            : AstError("Syntax Error", message) {}
    };

    class AstTypeError : public AstError {
    public:
        explicit AstTypeError(const std::string& message) 
            : AstError("Type Error", message) {}
    };
}