/***
 * @file tokens.hpp
 */

#pragma once

//////////////
// Includes //
//////////////

#include <iostream>
#include <vector>

//////////
// Code //
//////////

namespace Solar {
namespace Lexer {

    // Definitions
    enum class TokenType {
        Ident,

        Var,
        If, Else,

        Integer,
        Float,

        Semicolon,
        Comma,
        Dot,

        OpenParen, 
        CloseParen, 
        OpenBracket, 
        CloseBracket, 
        OpenBrace, 
        CloseBrace,

        Equals, // =
        Plus, // +
        Minus, // -
        Star, // *
        Slash, // "/"
        Pow, // ^
        Mod, // %

        EOF_,
    };

    struct TokenPos {
        size_t line;
        size_t start;
        size_t end;

        std::string toString() const {
            return "L(" + std::to_string(this->line) + ")" + std::to_string(this->start) + ":" + std::to_string(this->end);
        };
    };

    struct Token {
        TokenPos pos;
        TokenType type;
        std::string content;
    };

    // Constants
    const std::vector<std::pair<std::string, TokenType>> Keywords = {};

    // Functions
    inline std::string TToString(const TokenType& tt) {
        switch (tt) {
            case TokenType::Ident: return "Ident";
            case TokenType::Var: return "Var";
            case TokenType::If: return "If";
            case TokenType::Else: return "Else";
            case TokenType::Integer: return "Integer";
            case TokenType::Float: return "Float";
            case TokenType::Semicolon: return "Semicolon";
            case TokenType::Comma: return "Comma";
            case TokenType::Dot: return "Dot";
            case TokenType::OpenParen: return "OpenParen";
            case TokenType::CloseParen: return "CloseParen";
            case TokenType::OpenBracket: return "OpenBracket";
            case TokenType::CloseBracket: return "CloseBracket";
            case TokenType::OpenBrace: return "OpenBrace";
            case TokenType::CloseBrace: return "CloseBrace";
            case TokenType::Equals: return "Equals";
            case TokenType::Plus: return "Plus";
            case TokenType::Minus: return "Minus";
            case TokenType::Star: return "Star";
            case TokenType::Slash: return "Slash";
            case TokenType::Pow: return "Pow";
            case TokenType::Mod: return "Mod";
            case TokenType::EOF_: return "EOF";
            default: return "Unknown";
        }
    }
}
}