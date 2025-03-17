/***
 * @file tokenizer.cpp
 */

//////////////
// Includes //
//////////////

#include "lexer.hpp"

using namespace std;

//////////
// Code //
//////////

namespace Solar {
namespace Lexer {

    vector<Token> tokenize(string source, string file = "unknow") {
        Error::ErrorSesion errSession;
        vector<Token> tokens;
        size_t pos = 0;
        size_t line = 1;

        while (source.size() > 0) {
            pos += 1;

            // White Space
            if (isspace(source[0])) {
                if (source[0] == '\n') { pos = 0; line += 1; }
                source.erase(0, 1);
                continue;
            }

            // Comments
            if (source[0] == '/' && source.size() > 1 && source[1] == '/') {
                while (source.size() > 0 && source[0] != '\n') {
                    source.erase(0, 1);
                }
                continue;
            }

            // Numbers
            if (isdigit(source[0])) {
                size_t start = 0;
                bool hasDecimal = false;

                while (start < source.size() && (isdigit(source[start]) || source[start] == '.')) {
                    if (source[start] == '.') {
                        if (hasDecimal) break;
                        hasDecimal = true;
                    }
                    start++;
                }

                tokens.push_back(Token {{line, pos + start - 1}, TokenType::Number, source.substr(0, start)});
                source.erase(0, start);
                pos += start - 1;
                continue;
            }

            // Chars
            if (source[0] == '\'') {
                if (source.size() < 3 || source[2] != '\'') {
                    errSession.addError(
                        "Expected closing ' at: ",
                        file,
                        line,
                        pos
                    );
                    source.erase(0, 1);
                    pos += 1;
                    continue;
                }

                string chr(1, source[1]);
                tokens.push_back(Token {{line, pos}, TokenType::Char, chr});
                source.erase(0, 3);
                pos += 3;
                continue;
            }

            // Strings
            if (source[0] == '"') {
                size_t endPos = 1;
                string str;

                while (endPos < source.size()) {
                    if (source[endPos] == '"' && source[endPos - 1] != '\\') {
                        break;
                    }

                    str += source[endPos];
                    endPos++;
                }

                if (endPos >= source.size()) {
                    errSession.addError(
                        "Unterminated string",
                        file,
                        line,
                        pos
                    );
                    source.erase(0, 1);
                    pos += 1;
                    continue;
                }

                tokens.push_back(Token {{line, pos}, TokenType::String, str});
                source.erase(0, endPos + 1);
                pos += endPos + 1;
                continue;
            }

            // Identifiers/Keywords
            if (isalpha(source[0]) || source[0] == '_') {
                size_t start = 0;
                while (start < source.size() && (isalnum(source[start]) || source[start] == '_')) {
                    start++;
                }

                string word = source.substr(0, start);
                TokenType type = TokenType::Identfier;

                auto it = Keywords.find(word);
                if (it != Keywords.end()) {
                    type = it->second;
                }

                tokens.push_back(Token {{line, pos}, type, word});
                source.erase(0, start);
                pos += start - 1;
                continue;
            }

            // Double Operators
            if (source.size() > 1) {
                string op = source.substr(0, 2);
                bool found = true;

                TokenType type;
                if (op == "==") type = TokenType::Equals;
                else if (op == "!=") type = TokenType::NotEquals;
                else if (op == "<=") type = TokenType::LessEquals;
                else if (op == ">=") type = TokenType::GreaterEquals;
                else if (op == "&&") type = TokenType::And;
                else if (op == "||") type = TokenType::Or;
                else if (op == "++") type = TokenType::PlusPlus;
                else if (op == "--") type = TokenType::MinusMinus;
                else if (op == "+=") type = TokenType::PlusEquals;
                else if (op == "-=") type = TokenType::MinusEquals;
                else if (op == "*=") type = TokenType::StarEquals;
                else if (op == "/=") type = TokenType::SlashEquals;
                else if (op == "^=") type = TokenType::PowEquals;
                else if (op == "%=") type = TokenType::ModEquals;
                else if (op == "..") type = TokenType::DotDot;
                else found = false;

                if (found) {
                    tokens.push_back(Token {{line, pos}, type, op});
                    source.erase(0, 2);
                    pos++;
                    continue;
                }
            }

            // Single Characters
            TokenType type;
            switch (source[0]) {
                case '(': type = TokenType::OpenParen; break;
                case ')': type = TokenType::CloseParen; break;
                case '[': type = TokenType::OpenBracket; break;
                case ']': type = TokenType::CloseBracket; break;
                case '{': type = TokenType::OpenCurly; break;
                case '}': type = TokenType::CloseCurly; break;

                case '<': type = TokenType::Less; break;
                case '>': type = TokenType::Greater; break;

                case '.': type = TokenType::Dot; break;
                case ',': type = TokenType::Comma; break;
                case ';': type = TokenType::Semicolon; break;
                case ':': type = TokenType::Colon; break;

                case '?': type = TokenType::Question; break;
                case '=': type = TokenType::Assignment; break;
                case '+': type = TokenType::Plus; break;
                case '-': type = TokenType::Minus; break;
                case '*': type = TokenType::Star; break;
                case '/': type = TokenType::Slash; break;
                case '^': type = TokenType::Pow; break;
                case '%': type = TokenType::Mod; break;
                case '!': type = TokenType::Not; break;
                default: {
                    errSession.addError(
                        "Unexpected character: '" + 
                        string(1, source[0]) + "'",
                        file,
                        line,
                        pos
                    );
                    source.erase(0, 1);
                    pos += 1;
                    continue;
                }
            }

            tokens.push_back(Token {{line, pos}, type, string(1, source[0])});
            source.erase(0, 1);
        }

        errSession.debug();

        tokens.push_back(Token {{line, pos}, TokenType::EOF_, ""});
        return tokens;
    }

}
}
