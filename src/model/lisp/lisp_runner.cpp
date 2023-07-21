#include "lisp_runner.h"
#include "lisp_expression.h"
#include "lisp_value.h"
#include "lisp_runtime_error.h"

LispRunner::LispRunner(const LispExpression& expression): _expression(expression) {}

LispValue LispRunner::run() const {
  LispValue result;

  // TODO

  return result;
}
