/***
 * @file tokenizer.hpp
 */

#pragma once

//////////////
// Includes //
//////////////

#include <cstddef>
#include <stdexcept>
#include "solar_pack.hpp"

using namespace std;

//////////
// Code //
//////////

namespace Solar {
namespace Lexer {

    std::vector<Token> tokenize(string source);

}
}