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

/***
 * @brief Evaluate a Var declaration
 * @param varDeclaration The var declaration to evaluate ( std::unique_ptr<VarDeclaration> )
 * @param Env The environment ( Env& )
 * @return The value of Variable, To make "var x = var y = 10;" possible
 */
RuntimeValueV evaluateVarDeclaration(std::unique_ptr<VarDeclaration> varDeclaration, Env& env);