#ifndef LISP_EXECUTION_CONTEXT_ADDITION_INCLUDED
#define LISP_EXECUTION_CONTEXT_ADDITION_INCLUDED

#include "lisp_execution_context_error.h"
#include "lisp_function_execution_context.h"
#include "lisp_value.h"
#include "lisp_value_factory.h"

class LispExecutionContextAddition : public LispFunctionExecutionContext {
public:
  LispExecutionContextAddition() = default;
  virtual ~LispExecutionContextAddition() = default;

  virtual LispValuePtr value(const LispValuePtrVector &func,
                             const LispExecutionContext &execution_context,
                             const std::any &context_param) {
    ensure_params(func);

    LispValuePtrVector params = extract_params(func);

    LispValue::DoubleType result = 0.0;

    params = execute_functions_and_extract_list_results(
        params, execution_context, context_param);

    for (const auto &param : params) {
      LispValuePtr value(
          expect_number(param, execution_context, context_param));
      result += value->to_double();
    }

    return LispValueFactory::new_double(result);
  }
};

#endif
