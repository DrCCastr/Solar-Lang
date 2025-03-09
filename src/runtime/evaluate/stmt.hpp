/***
 * @file stmt.hpp
 * @brief Runtime evaluation of statements.
 */

#pragma once

#include "runtime/value.hpp"
#include "runtime/runner.hpp"
#include <memory>

/***
 * @brief Evaluate a Program
 * @param program The statement to evaluate ( std::unique_ptr<Stmt> )
 * @param env The environment ( Env& )
 * @return The result of the Program
 */
RuntimeValueV evaluateProgram(std::unique_ptr<Program> program, Env& env);