#pragma once

#include <string>
#include <memory>
#include <vector>

enum class TokenEnum {
    Keyword,
    Ident, Number,
    Semicolon,
    Equals,
    LParam, RParam,
    BinaryOp,
    FE
};

std::ostream& operator<<(std::ostream& os, const TokenEnum& tokenEnum);

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

std::vector<Token> lexerParse(const std::string& source);
