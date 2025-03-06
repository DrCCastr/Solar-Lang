#pragma once

#include "ast/nodes.hpp"
#include "value.hpp"
#include "evaluate/stmt.hpp"
#include "evaluate/expr.hpp"
#include <cmath>
#include <memory>

std::unique_ptr<RuntimeValue> evaluateNode(std::unique_ptr<Stmt> astNode);