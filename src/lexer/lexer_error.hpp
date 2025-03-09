/***
 * @file lexer_error.hpp
 * @brief Defines the Lexer error classes.
 */

#pragma once

#include "error_base.hpp"

/***
 * @namespace Error
 * @brief Implements Lexer errors to namespace.
 */
namespace Error {
    class LexerError : public BaseError {
    public:
        explicit LexerError(const std::string& subType, const std::string& message) 
            : BaseError("( Lexer ) " + subType, message) {}
    };

    class IlegalCharacter : public LexerError {
    public:
        explicit IlegalCharacter(const std::string& message)
            : LexerError("Ilegal Character", message) {}
    };
}