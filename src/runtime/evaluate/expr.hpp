#pragma once

#include "runtime/value.hpp"
#include "runtime/runner.hpp"
#include "runtime/runtime_error.hpp"
#include <memory>

std::unique_ptr<RuntimeValue> evaluateBinop(std::unique_ptr<BinaryExpr> binop);