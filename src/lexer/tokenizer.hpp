/***
 * @file tokenizer.hpp
 */

#pragma once

//////////////
// Includes //
//////////////

#include <stdexcept>
#include "tokens.hpp"

//////////
// Code //
//////////

namespace Solar {
namespace Lexer {

    std::vector<Token> tokenize(std::string source);

}
}