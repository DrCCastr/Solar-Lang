/***
 * @file tokens.hpp
 */

#pragma once

//////////////
// Includes //
//////////////

#include <cstddef>
#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

//////////
// Code //
//////////

namespace Solar {
namespace Lexer {

    // Definitions
    enum class TokenType {
        EOF_,
        Number,
        Char,
        String,
        Identfier,

        OpenBracket,
        CloseBracket,
        OpenCurly,
        CloseCurly,
        OpenParen,
        CloseParen,

        Assignment, // =
        Equals,     // ==
        Not,        // !
        NotEquals,  // !=

        Less,
        LessEquals,
        Greater,
        GreaterEquals,

        Or,
        And,

        Dot,
        DotDot,
        Semicolon,
        Colon,
        Question,
        Comma,

        PlusPlus,
        MinusMinus,
        PlusEquals,
        MinusEquals,
        SlashEquals,
        StarEquals,
        PowEquals,
        ModEquals,

        Plus,
        Minus,
        Slash,
        Star,
        Pow,
        Mod,

        Let,
        Const,
        Class,
        Struct,
        New,
        Func,
        If,
        Else,
        Foreach,
        While,
        For,
        Export,
        Typeof,
        Type,
        In,
    };

    struct TokenPos {
        size_t line;
        size_t collum;

        string toString() const {
            return to_string(this->line) + ":" + to_string(this->collum);
        };
    };

    struct Token {
        TokenPos pos;
        TokenType type;
        string content;
    };

    // Constants
    const unordered_map<::string, TokenType> Keywords = {
        {"let", TokenType::Let},
        {"const", TokenType::Const},
        {"class", TokenType::Class},
        {"struct", TokenType::Struct},
        {"new", TokenType::New},
        {"func", TokenType::Func},
        {"if", TokenType::If},
        {"else", TokenType::Else},
        {"foreach", TokenType::Foreach},
        {"while", TokenType::While},
        {"for", TokenType::For},
        {"export", TokenType::Export},
        {"typeof", TokenType::Typeof},
        {"type", TokenType::Type},
        {"in", TokenType::In}
    };

    // Functions
    inline string TToString(const TokenType& tt) {
        switch (tt) {
            case TokenType::EOF_: return "EOF";
            case TokenType::Number: return "Number";
            case TokenType::Char: return "Char";
            case TokenType::String: return "String";
            case TokenType::Identfier: return "Identifier";

            case TokenType::OpenBracket: return "OpenBracket";
            case TokenType::CloseBracket: return "CloseBracket";
            case TokenType::OpenCurly: return "OpenCurly";
            case TokenType::CloseCurly: return "CloseCurly";
            case TokenType::OpenParen: return "OpenParen";
            case TokenType::CloseParen: return "CloseParen";

            case TokenType::Assignment: return "Assignment";
            case TokenType::Equals: return "Equals";
            case TokenType::Not: return "Not";
            case TokenType::NotEquals: return "NotEquals";

            case TokenType::Less: return "Less";
            case TokenType::LessEquals: return "LessEquals";
            case TokenType::Greater: return "Greater";
            case TokenType::GreaterEquals: return "GreaterEquals";

            case TokenType::Or: return "Or";
            case TokenType::And: return "And";

            case TokenType::Dot: return "Dot";
            case TokenType::DotDot: return "DotDot";
            case TokenType::Semicolon: return "Semicolon";
            case TokenType::Colon: return "Colon";
            case TokenType::Question: return "Question";
            case TokenType::Comma: return "Comma";

            case TokenType::PlusPlus: return "PlusPlus";
            case TokenType::MinusMinus: return "MinusMinus";
            case TokenType::PlusEquals: return "PlusEquals";
            case TokenType::MinusEquals: return "MinusEquals";
            case TokenType::SlashEquals: return "SlashEquals";
            case TokenType::StarEquals: return "StarEquals";
            case TokenType::PowEquals: return "PowEquals";
            case TokenType::ModEquals: return "ModEquals";

            case TokenType::Plus: return "Plus";
            case TokenType::Minus: return "Minus";
            case TokenType::Slash: return "Slash";
            case TokenType::Star: return "Star";
            case TokenType::Pow: return "Pow";
            case TokenType::Mod: return "Mod";

            case TokenType::Let: return "Let";
            case TokenType::Const: return "Const";
            case TokenType::Class: return "Class";
            case TokenType::Struct: return "Struct";
            case TokenType::New: return "New";
            case TokenType::Func: return "Func";
            case TokenType::If: return "If";
            case TokenType::Else: return "Else";
            case TokenType::Foreach: return "Foreach";
            case TokenType::While: return "While";
            case TokenType::For: return "For";
            case TokenType::Export: return "Export";
            case TokenType::Typeof: return "Typeof";
            case TokenType::Type: return "Type";
            case TokenType::In: return "In";
            default: return "Unknown";
        }
    }

}
}