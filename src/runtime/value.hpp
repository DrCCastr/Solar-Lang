#pragma once
#include <string>

enum class ValueType {
    Null,
    Number
};

class RuntimeValue {
public:
    virtual ~RuntimeValue() = default;
    virtual ValueType getType() const = 0;
    virtual std::string toString() const = 0;
};

class NullValue : public RuntimeValue {
public:
    ValueType getType() const override { return ValueType::Null; }
    std::string toString() const override { return "Null"; }
};

class NumberValue : public RuntimeValue {
public:
    NumberValue(double value) : value(value) {}

    ValueType getType() const override { return ValueType::Number; }
    std::string toString() const override { return "Number: " + std::to_string(value); }

    double value;
};