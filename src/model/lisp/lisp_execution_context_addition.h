#ifndef LISP_EXECUTION_CONTEXT_ADDITION_INCLUDED
#define LISP_EXECUTION_CONTEXT_ADDITION_INCLUDED

#include "lisp_execution_context_error.h"
#include "lisp_function.h"
#include "lisp_function_execution_context.h"

class LispExecutionContextAddition : public LispFunctionExecutionContext {
public:
  LispExecutionContextAddition() = default;
  virtual ~LispExecutionContextAddition() = default;

  virtual LispValue value(const LispFunction &func,
                          const LispExecutionContext &execution_context,
                          const std::any &context_param = {}) {
    ensure_params(func);

    double result = 0.0;

    LispValuePtrVector params = execute_functions_and_extract_list_results(
        func.params(), execution_context);

    for (const auto &param : params) {
      LispValue value(expect_number(param, execution_context));
      result += value.number();
    }

    return LispValue(result);
  }
};

#endif
