#ifndef LISP_EXECUTION_CONTEXT_DIVISION_INCLUDED
#define LISP_EXECUTION_CONTEXT_DIVISION_INCLUDED

#include "lisp_execution_context_error.h"
#include "lisp_function_execution_context.h"
#include "lisp_value.h"

class LispExecutionContextDivision : public LispFunctionExecutionContext {
public:
  LispExecutionContextDivision() = default;
  virtual ~LispExecutionContextDivision() = default;

  virtual LispValue value(const LispFunction &func, const LispExecutionContext& execution_context) {
    ensure_params(func);

    double result;

    const auto &first_param_opt = func.param_at(0);
    if (!first_param_opt) {
      throw LispExecutionContextError("Unexpected: no parameters");
    }
    const auto &first_param = *first_param_opt;

    LispValue value;
    if (first_param->is_function()) {
      value = execution_context.execute(*first_param);
    } else if (first_param->is_number()) {
      value = *first_param;
    } else {
      throw LispExecutionContextError(
          "Unable to perform division with this lisp value");
    }

    // First parameter of subtraction is base value
    result = value.number();

    // Skip first param
    for (size_t n = 1; n < func.param_count(); n++) {
      const auto &param_opt = func.param_at(n);
      if (!param_opt) {
        break;
      }

      const auto &param = *param_opt;
      LispValue value;
      if (param->is_function()) {
        value = execution_context.execute(*param);
      } else if (param->is_number()) {
        value = *param;
      } else {
        throw LispExecutionContextError(
            "Unable to perform division with this lisp value");
      }

      if (value.number() == 0.0) {
        throw LispExecutionContextError("Division by zero");
      }

      result /= value.number();
    }

    return LispValue(result);
  }
};

#endif
