#ifndef LISP_EXECUTION_CONTEXT_DIVISION_INCLUDED
#define LISP_EXECUTION_CONTEXT_DIVISION_INCLUDED

#include "lisp_execution_context_error.h"
#include "lisp_function_execution_context.h"
#include "lisp_value.h"

class LispExecutionContextDivision : public LispFunctionExecutionContext {
public:
  LispExecutionContextDivision() = default;
  virtual ~LispExecutionContextDivision() = default;

  virtual LispValuePtr value(const LispValuePtrVector &func,
                             const LispExecutionContext &execution_context,
                             const std::any &context_param) {
    ensure_params(func);

    LispValuePtrVector params = extract_params(func);

    LispValue::DoubleType result;

    params = execute_functions_and_extract_list_results(
        params, execution_context, context_param);

    const auto &first_param = params[0];

    LispValuePtr value(
        expect_number(first_param, execution_context, context_param));

    // First parameter of subtraction is base value
    result = value->to_double();

    // Skip first param
    for (size_t n = 1; n < params.size(); n++) {
      const auto &param = params[n];
      LispValuePtr value(
          expect_number(param, execution_context, context_param));

      if (value->to_double() == 0.0) {
        throw LispExecutionContextError("Division by zero");
      }

      result /= value->to_double();
    }

    return LispValueFactory::new_double(result);
  }
};

#endif
