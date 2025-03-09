/***
 * @file stmt.cpp
 * @brief Runtime evaluation of statements.
 */

#include "stmt.hpp"

/***
 * @brief Evaluate a program
 * @param program The statement to evaluate ( std::unique_ptr<Stmt> )
 * @param env The environment ( Env& )
 * @return The result of the program
 */

RuntimeValueV evaluateProgram(std::unique_ptr<Program> program, Env& env) {
  RuntimeValueV lastEvaluated = NullValue();

  for (size_t i = 0; i < program->body.size(); i++) {
    lastEvaluated = evaluateNode(std::move(program->body[i]), env);
  }

  return lastEvaluated;
}