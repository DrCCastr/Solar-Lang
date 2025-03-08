/***
 * @file runtime_error.hpp
 * @brief Defines the Runtime error classes.
 */

#pragma once

#include "error_base.hpp"

/***
 * @namespace SolarError
 * @brief Implements Runtime errors to namespace.
 */
namespace SolarError {
    class RuntimeError : public BaseError {
    public:
        RuntimeError(const std:: string& subType, const std::string& message) 
            : BaseError("( Runtime ) " + subType, message) {}
    };

    class RuntimeTypeError : public RuntimeError {
    public:
        RuntimeTypeError(const std::string& message) 
            : RuntimeError("Type Error", message) {}
    };
}