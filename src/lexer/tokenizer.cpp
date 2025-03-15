/***
 * @file tokenizer.cpp
 */

//////////////
// Includes //
//////////////

#include "tokenizer.hpp"

//////////
// Code //
//////////

namespace Solar {
namespace Lexer {

    std::vector<Token> tokenize(std::string source) {
        std::vector<Token> tokens;

        size_t pos = 0;
        size_t line = 1;

        while (source.size() > 0) {
            pos += 1;

            if (isspace(source[0])) {
                if (source[0] == '\n') { pos = 0; line += 1; }
                source.erase(0, 1);
                continue;
            }

            if (isdigit(source[0])) {
                size_t start = 0;
                bool isFloat = false;

                while (isdigit(source[start]) || source[start] == '.') {
                    if (source[start] == '.') {
                        if (isFloat) break; // One dot
                        isFloat = true;
                    }
                    start++;
                }

                std::string number = source.substr(0, start);
                tokens.push_back(Token {{line, pos, pos + start - 1}, isFloat ? TokenType::Float : TokenType::Integer, number});
                source.erase(0, start);
                pos += start - 1;
                continue;
            }

            switch (source[0]) {
                case '(': tokens.push_back(Token {{line, pos, pos}, TokenType::OpenParen, "("}); break;
                case ')': tokens.push_back(Token {{line, pos, pos}, TokenType::CloseParen, ")"}); break;
                case '{': tokens.push_back(Token {{line, pos, pos}, TokenType::OpenBrace, "{"}); break;
                case '}': tokens.push_back(Token {{line, pos, pos}, TokenType::CloseBrace, "}"}); break;

                case ',': tokens.push_back(Token {{line, pos, pos}, TokenType::Comma, ","}); break;
                case '.': tokens.push_back(Token {{line, pos, pos}, TokenType::Dot, "."}); break;

                case '=': tokens.push_back(Token {{line, pos, pos}, TokenType::Equals, "="}); break;
                case '-': tokens.push_back(Token {{line, pos, pos}, TokenType::Minus, "-"}); break;
                case '+': tokens.push_back(Token {{line, pos, pos}, TokenType::Plus, "+"}); break;
                case ';': tokens.push_back(Token {{line, pos, pos}, TokenType::Semicolon, ";"}); break;
                case '*': tokens.push_back(Token {{line, pos, pos}, TokenType::Star, "*"}); break;
                default: {
                    throw std::runtime_error(
                        "Unexpected character: '" +
                        std::string(1, source[0]) + 
                        "', Error at: " + (TokenPos {line, pos, pos}).toString()
                    );
                }
            }

            source.erase(0, 1);
        }

        tokens.push_back(Token {{0, 0, 0}, TokenType::EOF_, ""});
        return tokens;
    }

}
}