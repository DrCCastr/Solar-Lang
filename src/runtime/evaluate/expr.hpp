/***
 * @file expr.hpp
 * @brief Runtime evaluation of expressions.
 */

#pragma once

#include "runtime/value.hpp"
#include "runtime/runner.hpp"
#include "runtime/runtime_error.hpp"
#include <memory>

/***
 * @brief Evaluate a binary expression
 * @param binop The binary expression to evaluate ( std::unique_ptr<BinaryExpr> )
 * @return The result of the binary operation
 */
std::unique_ptr<RuntimeValue> evaluateBinop(std::unique_ptr<BinaryExpr> binop);