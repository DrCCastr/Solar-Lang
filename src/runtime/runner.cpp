/***
 * @file runner.cpp
 * @brief Implementation of runtime interpreter.
 */

#include "runner.hpp"

/***
 * @brief Evaluate a program
 * @param program The program to evaluate ( std::unique_ptr<Stmt> )
 * @return The result of the program
 */
std::unique_ptr<RuntimeValue> evaluateNode(std::unique_ptr<Stmt> astNode) {
    switch (astNode->getKind()) {
        case NodeType::Program: {
            return evaluateProgram(
                std::unique_ptr<Program>(
                    static_cast<Program*>(astNode.release())
                )
            );
        }

        case NodeType::NumericLiteral: {
            const auto& numLit = static_cast<const NumericLiteral&>(*astNode);
            return std::make_unique<NumberValue>(numLit.value);
        }

        case NodeType::NullLiteral: {
            return std::make_unique<NullValue>();
        }

        case NodeType::Identifier: {
            return std::make_unique<NullValue>(); // TODO: Implement
        }

        case NodeType::BinaryExpr: {
            return evaluateBinop(
                std::unique_ptr<BinaryExpr>(
                    static_cast<BinaryExpr*>(astNode.release())
                )
            );
        }

        default: {
            throw SolarError::RuntimeError("Unexpected", "Unknown node: " + astNode->toString());
        }
    }
}