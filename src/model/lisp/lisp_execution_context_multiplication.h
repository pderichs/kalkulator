#ifndef LISP_EXECUTION_CONTEXT_MULTIPLICATION_INCLUDED
#define LISP_EXECUTION_CONTEXT_MULTIPLICATION_INCLUDED

#include "lisp_execution_context_error.h"
#include "lisp_function_execution_context.h"
#include "lisp_value.h"

class LispExecutionContextMultiplication : public LispFunctionExecutionContext {
public:
  LispExecutionContextMultiplication() = default;
  virtual ~LispExecutionContextMultiplication() = default;

  virtual LispValue value(const LispFunction &func, const LispExecutionContext& execution_context) {
    ensure_params(func);

    double result = 1.0;

    for (const auto &param : func.params()) {
      LispValue value(expect_number(param, execution_context));
      result *= value.number();
    }

    return LispValue(result);
  }
};

#endif
