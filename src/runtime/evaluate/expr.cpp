#include "expr.hpp"

std::unique_ptr<RuntimeValue> evaluateBinop(std::unique_ptr<BinaryExpr> binop) {
    auto leftResult = evaluateNode(std::move(binop->left));
    auto rightResult = evaluateNode(std::move(binop->right));

    if (leftResult->getType() != ValueType::Number
    || rightResult->getType() != ValueType::Number) {
        throw SolarError::RuntimeTypeError("");
    }

    double left = static_cast<NumberValue*>(leftResult.get())->value;
    double right = static_cast<NumberValue*>(rightResult.get())->value;

    double result;
    switch (binop->op[0]) {
        case '+': result = left + right; break;
        case '-': result = left - right; break;
        case '*': result = left * right; break;
        case '%': result = std::fmod(left, right); break;
    }

    NumberValue numValue(result);
    return std::make_unique<NumberValue>(numValue);
}