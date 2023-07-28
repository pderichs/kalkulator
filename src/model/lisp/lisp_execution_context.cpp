#include "lisp_execution_context.h"
#include "lisp_execution_context_addition.h"
#include "lisp_execution_context_division.h"
#include "lisp_execution_context_error.h"
#include "lisp_execution_context_multiplication.h"
#include "lisp_execution_context_subtraction.h"
#include "lisp_function.h"
#include "lisp_function_execution_context.h"
#include "lisp_value.h"
#include <memory>

LispExecutionContext::LispExecutionContext() {
  // Prepare functions
  _functions["+"] = std::make_shared<LispExecutionContextAddition>();
  _functions["-"] = std::make_shared<LispExecutionContextSubtraction>();
  _functions["*"] = std::make_shared<LispExecutionContextMultiplication>();
  _functions["/"] = std::make_shared<LispExecutionContextDivision>();
}

LispValue LispExecutionContext::execute(const LispValue &value) const {
  if (!value.is_function()) {
    return value;
  }

  LispValue execution_result = eval_function(value.function());
  return execution_result;
}

LispValue LispExecutionContext::eval_function(const LispFunction &func) const {
  const auto &execution_context_it = _functions.find(func.identifier());
  if (execution_context_it == _functions.end()) {
    throw LispExecutionContextError("Unknown function identifier");
  }

  const auto &execution_context = execution_context_it->second;
  return execution_context->value(func, *this);
}

void LispExecutionContext::add_function(
    const std::string &identifier, const LispFunctionExecutionContextPtr func) {
  if (!func) {
    throw LispExecutionContextError("Invalid function pointer");
  }

  const auto &execution_context_it = _functions.find(identifier);
  if (execution_context_it != _functions.end()) {
    throw LispExecutionContextError("Identifier is already assigned");
  }

  _functions[identifier] = func;
}
