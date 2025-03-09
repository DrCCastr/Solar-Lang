/***
 * @file env.hpp
 * @brief Declares the Environment class.
 */

#pragma once

#include "value.hpp"
#include "runtime_error.hpp"
#include <unordered_map>

/***
 * @class Env
 * @brief Represents a runtime environment.
 */
class Env {
public:
    Env(std::unique_ptr<Env> parent = nullptr) 
        : parent(std::move(parent)) {}

    RuntimeValueV declareValue(const std::string& name, RuntimeValueV value, const TokenPos& position) {
        if (store.find(name) != store.end()) {
            throw Error::InvalidExpressionError(
                "Variable " + name +
                " already declared in this scope, Error at: " +
                position.toString()
            );
        }

        store[name] = value;
        return value;
    }

    RuntimeValueV assignValue(const std::string& name, RuntimeValueV value, const TokenPos& position) {
        Env& env = resolve(name, position);
        auto it = env.store.find(name);

        if (it == env.store.end()) {
            throw Error::InvalidExpressionError(
                "Variable " + name +
                " not found in this environment, Error at: " +
                position.toString()
            );
        }

        it->second = value;
        return value;
    }

    RuntimeValueV lookupValue(const std::string& name, const TokenPos& position) {
        const Env& env = resolve(name, position);
        auto it = env.store.find(name);

        if (it == env.store.end()) {
            throw Error::InvalidExpressionError(
                "Variable " + name +
                " not found in this environment, Error at: " +
                position.toString()
            );
        }

        return it->second;
    }

    Env& resolve(const std::string& name, const TokenPos& position) {
        if (store.find(name) != store.end()) {
            return *this;
        }

        if (!parent) {
            throw Error::InvalidExpressionError(
                "Variable " + name +
                " not found in this environment, Error at: " +
                position.toString()
            );
        }

        return parent->resolve(name, position);
    }

private:
    std::unique_ptr<Env> parent;
    std::unordered_map<std::string, RuntimeValueV> store;
};