/***
 * @file stmt.hpp
 * @brief Runtime evaluation of statements.
 */

#pragma once

#include "runtime/value.hpp"
#include "runtime/runner.hpp"
#include <memory>

/***
 * @brief Evaluate a statement
 * @param stmt The statement to evaluate ( std::unique_ptr<Stmt> )
 * @return The result of the statement
 */
std::unique_ptr<RuntimeValue> evaluateProgram(std::unique_ptr<Program> program);