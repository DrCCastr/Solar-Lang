/***
 * @file value.hpp
 * @brief Defines the rutime values.
 */

#pragma once
#include <string>
#include <variant>

enum class ValueType {
    Null,
    Number,
    Bool
};

/***
 * @class RuntimeValue
 * @brief Represents a runtime value.
 */
class RuntimeValue {
public:
    virtual ~RuntimeValue() = default;

    /*** * @brief Get the type of the value. */
    virtual ValueType getType() const = 0;
    /*** * @brief Get the string representation of the type. */
    virtual std::string getTypeString() const = 0;
    /*** * @brief Get the string representation of the value. */
    virtual std::string toString() const = 0;
};

/***
 * @class NullValue
 * @brief Represents a null value.
 */
class NullValue : public RuntimeValue {
public:
    ValueType getType() const override { return ValueType::Null; }

    std::string getTypeString() const override { return "Null"; }
    std::string toString() const override { return "Null"; }
};

/***
 * @class NumberValue
 * @property value The value of the number ( double ).
 */
class NumberValue : public RuntimeValue {
public:
    NumberValue(double value = 0) : value(value) {}

    ValueType getType() const override { return ValueType::Number; }
    std::string getTypeString() const override { return "Number"; }
    std::string toString() const override { return "Number: " + std::to_string(value); }

    double value;
};


/***
 * @class BoolValue
 * @property value The value of the Boolean ( bool ).
 */
class BoolValue : public RuntimeValue {
public:
    BoolValue(bool value = false) : value(value) {}

    ValueType getType() const override { return ValueType::Bool; }
    std::string getTypeString() const override { return "Bool"; }
    std::string toString() const override { return "Boolean: " + std::to_string(value); }

    bool value;
};

// Define a variant type for runtime values.
// Example: RuntimeValueV function() { if (condition) return NullValue(); else return NumberValue(42); }
typedef std::variant<NullValue, NumberValue, BoolValue> RuntimeValueV;