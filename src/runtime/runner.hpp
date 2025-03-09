/***
 * @file runner.hpp
 * @brief Declaration of runtime interpreter.
 */

#pragma once

#include "ast/nodes.hpp"
#include "value.hpp"
#include "env.hpp"
#include "evaluate/stmt.hpp"
#include "evaluate/expr.hpp"
#include "runtime_error.hpp"
#include <memory>

/***
 * @brief Evaluate a program
 * @param program The program to evaluate ( std::unique_ptr<Stmt> )
 * @return The result of the program
 */
RuntimeValueV evaluateNode(std::unique_ptr<Stmt> astNode, Env& env);