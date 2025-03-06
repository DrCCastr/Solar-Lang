#pragma once

#include <stdexcept>
#include <string>

namespace SolarError {
    class BaseError : public std::runtime_error {
    protected:
        explicit BaseError(const std::string& type, const std::string& message) 
            : std::runtime_error(type + ": " + message) {}
    };
}