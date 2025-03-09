/***
 * @file tokens.hpp
 * @brief Defines the Token struct.
 */

#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <vector>

enum class TokenEnum {
    Ident,
    Var,

    Null,
    Number,

    Semicolon,

    LParam,
    RParam,

    BinaryOp,
    Equals,

    FE
};

/**
 * @brief Converts a TokenEnum to a string.
 * @param tokenEnum The TokenEnum to convert.
 * @return std::string The string representation of the TokenEnum.
 */
std::string TokenEnumString(const TokenEnum& tokenEnum);

struct TokenPos {
    size_t line;
    size_t start;
    size_t end;

    std::string toString() const {
        return std::to_string(line) + "-" + std::to_string(start) + ":" + std::to_string(end);
    }
};;

struct Token {
    TokenEnum type;
    std::string content;
    TokenPos pos;

    Token(TokenEnum type, const std::string& content, TokenPos pos)
        : type(type), content(content), pos(pos) {}
};