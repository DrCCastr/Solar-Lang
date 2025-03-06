#include "tokenizer.hpp"

static const std::vector<std::pair<std::string, TokenEnum>> keywords = {
    {"Var", TokenEnum::Var},
    {"Null", TokenEnum::Null}
};

TokenEnum isKeyword(const std::string& word) {
    for (const auto& keyword : keywords) {
        if (word == keyword.first) {
            return keyword.second;
        }
    }
    return TokenEnum::Ident;
}

void lexerPush(std::vector<Token>& tokenList, TokenEnum type, const std::string& content, TokenPos pos) {
    tokenList.emplace_back(type, content, pos);
}

bool validChar(char chr) {
    return (chr == '\t' || chr == ' ' || std::isdigit(chr) || std::isalpha(chr) || chr == '_' || chr == ';');
}

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
                    throw SolarError::IlegalCharacter(
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

    lexerPush(tokens, TokenEnum::FE, "", {0, 0, 0});
    return tokens;
}