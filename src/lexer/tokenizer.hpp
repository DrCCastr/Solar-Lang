/***
 * @file tokenizer.hpp
 * @brief Declares the Tokenizer.
 */

#pragma once

#include "tokens.hpp"
#include "lexer_error.hpp"
#include <vector>

std::vector<Token> lexerParse(const std::string& source);