/***
 * @file stmt.cpp
 * @brief Runtime evaluation of statements.
 */

#include "stmt.hpp"

/***
 * @brief Evaluate a statement
 * @param stmt The statement to evaluate ( std::unique_ptr<Stmt> )
 * @return The result of the statement
 */
std::unique_ptr<RuntimeValue> evaluateProgram(std::unique_ptr<Program> program) {
    std::unique_ptr<RuntimeValue> lastEvaluated = std::make_unique<NullValue>();

    for (size_t i = 0; i < program->body.size(); i++) {
        lastEvaluated = evaluateNode(std::move(program->body[i]));
    }

    return lastEvaluated;
}