#ifndef LISP_EXECUTION_CONTEXT_ADDITION_INCLUDED
#define LISP_EXECUTION_CONTEXT_ADDITION_INCLUDED

#include "lisp_execution_context_error.h"
#include "lisp_function_execution_context.h"

class LispExecutionContextAddition : public LispFunctionExecutionContext {
public:
  LispExecutionContextAddition() = default;
  virtual ~LispExecutionContextAddition() = default;

  virtual LispValue value(const LispFunction &func) {
    double result = 0.0;

    for (const auto& param: func.params()) {
      if (!param->is_number()) {
        throw LispExecutionContextError("Unable to perform addition with this lisp value");
      }

      result += param->number();
    }

    return LispValue(result);
  }
};

#endif
