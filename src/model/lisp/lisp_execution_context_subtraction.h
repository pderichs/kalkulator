#ifndef LISP_EXECUTION_CONTEXT_SUBTRACTION_INCLUDED
#define LISP_EXECUTION_CONTEXT_SUBTRACTION_INCLUDED

#include "lisp_execution_context.h"
#include "lisp_execution_context_error.h"
#include "lisp_function_execution_context.h"
#include <cstddef>
#include <sstream>

class LispExecutionContextSubtraction : public LispFunctionExecutionContext {
public:
  LispExecutionContextSubtraction() = default;
  virtual ~LispExecutionContextSubtraction() = default;

  virtual LispValue value(const LispFunction &func,
                          const LispExecutionContext &execution_context,
                          const std::any &context_param = {}) {
    ensure_params(func);

    double result;

    LispValuePtrVector params = execute_functions_and_extract_list_results(
        func.params(), execution_context);

    const auto &first_param = params[0];

    LispValue value(expect_number(first_param, execution_context));

    // First parameter of subtraction is base value
    result = value.number();

    // Skip first param
    for (size_t n = 1; n < params.size(); n++) {
      const auto &param = params[n];
      LispValue value(expect_number(param, execution_context));
      result -= value.number();
    }

    return LispValue(result);
  }
};

#endif
