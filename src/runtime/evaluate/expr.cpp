/***
 * @file expr.cpp
 * @brief Runtime evaluation of expressions.
 */

#include "expr.hpp"

/***
 * @brief Evaluate a binary expression
 * @param binop The binary expression to evaluate ( std::unique_ptr<BinaryExpr> )
 * @param env The environment ( Env& )
 * @return The result of the binary operation
 */
RuntimeValueV evaluateBinop(std::unique_ptr<BinaryExpr> binop, Env& env) {
    NodePos leftPos = binop->left->position;
    NodePos rightPos = binop->right->position;

    auto leftResult = evaluateNode(std::move(binop->left), env);
    auto rightResult = evaluateNode(std::move(binop->right), env);

    bool isLeftNumber = std::visit([](auto&& arg) { 
        return std::is_same_v<std::decay_t<decltype(arg)>, NumberValue>; 
    }, leftResult);

    bool isRightNumber = std::visit([](auto&& arg) {
        return std::is_same_v<std::decay_t<decltype(arg)>, NumberValue>; 
    }, rightResult);

    if (!isLeftNumber || !isRightNumber) {
        std::string leftType = std::visit([](auto&& arg) { return arg.getTypeString(); }, leftResult);
        std::string rightType = std::visit([](auto&& arg) { return arg.getTypeString(); }, rightResult);
        
        throw Error::RuntimeTypeError(
            "Type " + leftType + " and " + rightType +
            " at: " + NodePos::combineNP(leftPos, rightPos).toString() +
            " are not supported for binary operations"
        );
    }

    double left = std::get<NumberValue>(leftResult).value;
    double right = std::get<NumberValue>(rightResult).value;

    double result;
    switch (binop->op[0]) {
        case '+': result = left + right; break;
        case '-': result = left - right; break;
        case '*': result = left * right; break;
        case '%': result = std::fmod(left, right); break;
    }

    return NumberValue(result);
}