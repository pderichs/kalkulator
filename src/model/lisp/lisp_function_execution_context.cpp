#include "lisp_function_execution_context.h"
#include "lisp_execution_context.h"
#include "lisp_function.h"
#include <sstream>

void LispFunctionExecutionContext::ensure_params(
    const LispFunction &func) const {
  if (func.param_count() == 0) {
    throw LispExecutionContextError("Unexpected: No params given");
  }
}

LispValue LispFunctionExecutionContext::expect_number(
    const LispValuePtr &value,
    const LispExecutionContext &execution_context) const {
  LispValue result;

  if (value->is_function()) {
    result = execution_context.execute(*value);
  } else if (value->is_number()) {
    result = *value;
  } else {
    std::stringstream ss;
    ss << "Unable to perform subtraction with this lisp value "
       << (int)value->type();
    throw LispExecutionContextError(ss.str());
  }

  return result;
}
