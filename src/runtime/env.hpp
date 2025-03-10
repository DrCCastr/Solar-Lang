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
    Env(std::unique_ptr<Env> parent = nullptr, bool defaultVals = false)
        : parent(std::move(parent)) {
        if (defaultVals) {
            this->declareValue("null", NullValue(), {0, 0, 0, 0});
            this->declareValue("false", BoolValue(false), {0, 0, 0, 0});
            this->declareValue("true", BoolValue(true), {0, 0, 0, 0});
        }
        }

    RuntimeValueV declareValue(const std::string& name, RuntimeValueV value, const NodePos& position) {
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

    RuntimeValueV assignValue(const std::string& name, RuntimeValueV value, const NodePos& position) {
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

    RuntimeValueV lookupValue(const std::string& name, const NodePos& position) {
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

    Env& resolve(const std::string& name, const NodePos& position) {
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