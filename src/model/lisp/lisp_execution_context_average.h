#ifndef LISP_EXECUTION_CONTEXT_AVG_INCLUDED
#define LISP_EXECUTION_CONTEXT_AVG_INCLUDED

#include "lisp_function_execution_context.h"
#include "lisp_value.h"

class LispExecutionContextAvg : public LispFunctionExecutionContext {
public:
  LispExecutionContextAvg() = default;
  virtual ~LispExecutionContextAvg() = default;

  virtual LispValuePtr value(const LispValuePtrVector &func,
                             const LispExecutionContext &execution_context,
                             const std::any &context_param) {
    ensure_params(func);

    LispValuePtrVector params = extract_params(func);

    LispValue::DoubleType result = 0.0;

    params = execute_functions_and_extract_list_results(
        params, execution_context, context_param);

    size_t param_count = params.size();

    for (const auto &param : params) {
      LispValuePtr value(
          expect_number(param, execution_context, context_param));
      result += value->to_double();
    }

    return LispValueFactory::new_double(result / param_count);
  }
};

#endif
