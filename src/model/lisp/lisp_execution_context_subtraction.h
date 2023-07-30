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
                          const LispExecutionContext &execution_context) {
    ensure_params(func);

    double result;

    const auto &first_param_opt = func.param_at(0);
    if (!first_param_opt) {
      throw LispExecutionContextError("Unexpected: no parameters");
    }
    const auto &first_param = *first_param_opt;

    LispValue value(expect_number(first_param, execution_context));

    // First parameter of subtraction is base value
    result = value.number();

    // Skip first param
    for (size_t n = 1; n < func.param_count(); n++) {
      const auto &param_opt = func.param_at(n);
      if (!param_opt) {
        break;
      }

      const auto &param = *param_opt;

      LispValue value(expect_number(param, execution_context));

      result -= value.number();
    }

    return LispValue(result);
  }
};

#endif
