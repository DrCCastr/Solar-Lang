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

}
}