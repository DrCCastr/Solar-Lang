/***
 * @file error_base.hpp
 * @brief Base error class for Solar
 */

#pragma once

#include <stdexcept>
#include <string>

/***
 * @namespace SolarError
 * @brief Namespace for Solar error handling
 */
namespace SolarError {
    class BaseError : public std::runtime_error {
    protected:
        explicit BaseError(const std::string& type, const std::string& message) 
            : std::runtime_error(type + ": " + message) {}
    };
}