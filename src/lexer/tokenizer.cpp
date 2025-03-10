/***
 * @file tokenizer.cpp
 * @brief Implements the Tokenizer.
 */

#include "tokenizer.hpp"

static const std::vector<std::pair<std::string, TokenEnum>> keywords = {
    {"var", TokenEnum::Var},
};

/***
 * @brief Checks if a word is a keyword.
 * @param word The word to check ( const std::string& ).
 * @return The token type of the keyword / identifier.
 */
TokenEnum isKeyword(const std::string& word) {
    for (const auto& keyword : keywords) {
        if (word == keyword.first) {
            return keyword.second;
        }
    }
    return TokenEnum::Ident;
}

/***
 * @brief Pushes a token to the token list.
 * @param tokenList The token list ( std::vector<Token>& ).
 * @param type The token type ( TokenEnum ).
 * @param content The token content ( const std::string& ).
 * @param pos The token position ( TokenPos ).
 */
void lexerPush(std::vector<Token>& tokenList, TokenEnum type, const std::string& content, TokenPos pos) {
    tokenList.emplace_back(type, content, pos);
}

/***
 * @brief Checks if a character is valid.
 * @param chr The character to check ( char ).
 * @return True if the character is valid, false otherwise.
 */
bool validChar(char chr) {
    return (chr == '\t' || chr == ' ' || std::isdigit(chr) || std::isalpha(chr) || chr == '_' || chr == ';');
}

/***
 * @brief Parses a source code string and returns a list of tokens.
 * @param source The source code string ( const std::string& ).
 * @return The list of tokens ( std::vector<Token> ).
 */
std::vector<Token> lexerParse(const std::string& source) {
    std::vector<Token> tokens;
    
    if (source.empty()) return tokens;

    size_t pos = 0;
    size_t line = 1;
    size_t len = source.length();

    for (size_t i = 0; i < len; i++) {
        char current = source[i];
        pos++;

        switch (current) {
            case ';': lexerPush(tokens, TokenEnum::Semicolon, ";", {line, pos, pos}); break;
            case '=': lexerPush(tokens, TokenEnum::Equals, "=", {line, pos, pos}); break;
            case '(': lexerPush(tokens, TokenEnum::LParam, "(", {line, pos, pos}); break;
            case ')': lexerPush(tokens, TokenEnum::RParam, ")", {line, pos, pos}); break;
            case '+': case '-': case '*': case '/': case '%':
                lexerPush(tokens, TokenEnum::BinaryOp, std::string(1, current), {line, pos, pos});
                break;
            case '\n':
                line++;
                pos = 0;
                break;

            default:
                if (!validChar(current)) {
                    throw Error::IlegalCharacter(
                        "Illegal character '" +
                        std::string(1, current) + 
                        "' at " + std::to_string(line) + 
                        "-" + 
                        std::to_string(pos) + 
                        ":" + std::to_string(pos)
                    );
                }
                break;
        }

        if (std::isdigit(current)) {
            size_t start_pos = pos;
            size_t start = i;

            while (i < len && (std::isdigit(source[i]) || source[i] == '.')) {
                i++;
                pos++;
            }

            lexerPush(tokens, TokenEnum::Number, source.substr(start, i - start), {line, start_pos, pos - 1});
            i--;
        }

        if (std::isalpha(current) || current == '_') {
            size_t start_pos = pos;
            size_t start = i;

            while (i < len && (std::isalnum(source[i]) || source[i] == '_')) {
                i++;
                pos++;
            }

            lexerPush(tokens, isKeyword(source.substr(start, i - start)), source.substr(start, i - start), {line, start_pos, pos - 1});
            i--;
        }
    }

    // EOF ( End of File ) Token
    lexerPush(tokens, TokenEnum::FE, "", {0, 0, 0});
    return tokens;
}