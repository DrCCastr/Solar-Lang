/***
 * @file runner.cpp
 * @brief Implementation of runtime interpreter.
 */

#include "runner.hpp"

/***
 * @brief Evaluate a program
 * @param program The program to evaluate ( std::unique_ptr<Stmt> )
 * @param env The environment to evaluate in
 * @return The result of the program
 */
RuntimeValueV evaluateNode(std::unique_ptr<Stmt> astNode, Env& env) {
    switch (astNode->getKind()) {
        ///////////////
        // Statments //
        ///////////////
        case NodeType::Program: {
            return evaluateProgram(
                std::unique_ptr<Program>(
                    static_cast<Program*>(astNode.release())
                ),
                env
            );
        }
        case NodeType::VarDeclaration: {
            
        }

        /////////////////
        // Expressions //
        /////////////////

        // Literals
        case NodeType::NumericLiteral: {
            const auto& numLit = static_cast<const NumericLiteral&>(*astNode);
            return NumberValue(numLit.value);
        }
        case NodeType::Identifier: {
            const auto& ident = static_cast<const Identifier&>(*astNode);
            return env.lookupValue(ident.name, {0, 0, 0});
        }
        // Etc
        case NodeType::BinaryExpr: {
            return evaluateBinop(
                std::unique_ptr<BinaryExpr>(
                    static_cast<BinaryExpr*>(astNode.release())
                ),
                env
            );
        }

        default: {
            throw Error::RuntimeError(
                "Unexpected", 
                "Unknown node type: " + std::to_string(static_cast<int>(astNode->getKind())) +
                " at " + astNode->position.toString()
            );
        }
    }
}