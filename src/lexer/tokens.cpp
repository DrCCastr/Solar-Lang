#include "tokens.hpp"

std::string TokenEnumString(const TokenEnum& tokenEnum) {
    switch (tokenEnum) {
        case TokenEnum::Ident: return "Identifier"; break;
        case TokenEnum::Var: return "Var"; break;

        case TokenEnum::Null: return "Null"; break;
        case TokenEnum::Number: return "Number"; break;

        case TokenEnum::Semicolon: return "Semicolon ( ; )"; break;

        case TokenEnum::LParam: return "Open Parantheses"; break;
        case TokenEnum::RParam: return "Close Parentheses"; break;

        case TokenEnum::BinaryOp: return "Binary Operator"; break;
        case TokenEnum::Equals: return "Equals ( = )"; break;

        case TokenEnum::FE: return "End of File"; break;

        default: return "Unknown"; break;
    }
}