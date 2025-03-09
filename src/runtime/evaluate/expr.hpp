/***
 * @file expr.hpp
 * @brief Runtime evaluation of expressions.
 */

#pragma once

#include "runtime/value.hpp"
#include "runtime/runner.hpp"
#include "runtime/runtime_error.hpp"
#include "runtime/env.hpp"
#include <memory>
#include <cmath>

/***
 * @brief Evaluate a binary expression
 * @param binop The binary expression to evaluate ( std::unique_ptr<BinaryExpr> )
 * @param env The environment ( Env& )
 * @return The result of the binary operation
 */
RuntimeValueV evaluateBinop(std::unique_ptr<BinaryExpr> binop, Env& env);