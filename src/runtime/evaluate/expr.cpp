#include "expr.hpp"

std::unique_ptr<RuntimeValue> evaluateBinop(std::unique_ptr<BinaryExpr> binop) {
    NodePos leftPos = binop->left->position;
    NodePos rightPos = binop->right->position;

    auto leftResult = evaluateNode(std::move(binop->left));
    auto rightResult = evaluateNode(std::move(binop->right));

    if (leftResult->getType() != ValueType::Number
    || rightResult->getType() != ValueType::Number) {
        throw SolarError::RuntimeTypeError(
        "Type " +
        leftResult->getTypeString() +
        " and " +
        rightResult->getTypeString() +
        " at: " + NodePos::combineNP(leftPos, rightPos).toString() +
        " are not supported for binary operations, add a method to the type to support this operation."
        );
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

    return std::make_unique<NumberValue>(result);
}