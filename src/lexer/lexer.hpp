/***
 * @file tokenizer.hpp
 */

#pragma once

//////////////
// Includes //
//////////////

#include "pack.hpp"
#include "error.hpp"
#include <string>
#include <cstddef>
#include <stdexcept>

using namespace std;

//////////
// Code //
//////////

namespace Solar {

    std::vector<Token> tokenize(string source, string file = "Unknow");

}