#ifndef LISP_EXECUTION_CONTEXT_MULTIPLICATION_INCLUDED
#define LISP_EXECUTION_CONTEXT_MULTIPLICATION_INCLUDED

#include "lisp_execution_context_error.h"
#include "lisp_function_execution_context.h"

class LispExecutionContextMultiplication : public LispFunctionExecutionContext {
public:
  LispExecutionContextMultiplication() = default;
  virtual ~LispExecutionContextMultiplication() = default;

  virtual LispValue value(const LispFunction &func) {
    double result = 1.0;

    for (const auto& param : func.params()) {
      if (!param->is_number()) {
        throw LispExecutionContextError("Unable to perform multiplication with this lisp value");
      }

      result *= param->number();
    }

    return LispValue(result);
  }
};

#endif
