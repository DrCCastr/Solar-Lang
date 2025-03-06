#include "stmt.hpp"

std::unique_ptr<RuntimeValue> evaluateProgram(std::unique_ptr<Program> program) {
    std::unique_ptr<RuntimeValue> lastEvaluated = std::make_unique<NullValue>();

    for (size_t i = 0; i < program->body.size(); i++) {
        lastEvaluated = evaluateNode(std::move(program->body[i]));
    }

    return lastEvaluated;
}

std::unique_ptr<RuntimeValue> evaluateStmt(std::unique_ptr<Stmt> stmt) {
    return evaluateNode(std::move(stmt));
}