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

    LispValue value;
    if (first_param->is_function()) {
      value = execution_context.execute(first_param->function());
    } else if (first_param->is_number()) {
      value = *first_param;
    } else {
      std::stringstream ss;
      ss << "Unable to perform subtraction with this lisp value "
         << (int)first_param->type();
      throw LispExecutionContextError(ss.str());
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
        std::stringstream ss;
        ss << "Unable to perform subtraction with this lisp value "
           << (int)param->type();
        throw LispExecutionContextError(ss.str());
      }

      result -= value.number();
    }

    return LispValue(result);
  }
};

#endif
