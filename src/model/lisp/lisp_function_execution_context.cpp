#include "lisp_function_execution_context.h"
#include "lisp_execution_context.h"

void LispFunctionExecutionContext::ensure_params(
    const LispFunction &func) const {
  if (func.param_count() == 0) {
    throw LispExecutionContextError("Unexpected: No params given");
  }
}
