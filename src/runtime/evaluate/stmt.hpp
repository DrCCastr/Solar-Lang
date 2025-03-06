#pragma once

#include "runtime/value.hpp"
#include "runtime/runner.hpp"
#include <memory>

std::unique_ptr<RuntimeValue> evaluateProgram(std::unique_ptr<Program> program);