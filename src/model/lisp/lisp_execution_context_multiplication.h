#ifndef LISP_EXECUTION_CONTEXT_MULTIPLICATION_INCLUDED
#define LISP_EXECUTION_CONTEXT_MULTIPLICATION_INCLUDED

#include "lisp_execution_context_error.h"
#include "lisp_function.h"
#include "lisp_function_execution_context.h"
#include "lisp_value.h"

class LispExecutionContextMultiplication : public LispFunctionExecutionContext {
public:
  LispExecutionContextMultiplication() = default;
  virtual ~LispExecutionContextMultiplication() = default;

  virtual LispValue value(const LispFunction &func,
                          const LispExecutionContext &execution_context,
                          const std::any &context_param = {}) {
    ensure_params(func);

    double result = 1.0;

    LispValuePtrVector params = execute_functions_and_extract_list_results(
        func.params(), execution_context);

    for (const auto &param : params) {
      LispValue value(expect_number(param, execution_context));
      result *= value.number();
    }

    return LispValue(result);
  }
};

#endif
