#ifndef LISP_EXECUTION_CONTEXT_REST_INCLUDED
#define LISP_EXECUTION_CONTEXT_REST_INCLUDED

#include "lisp_function_execution_context.h"
#include "lisp_value.h"
#include "lisp_value_factory.h"

class LispExecutionContextRest : public LispFunctionExecutionContext {
public:
  LispExecutionContextRest() = default;
  virtual ~LispExecutionContextRest() = default;

  virtual LispValuePtr value(const LispValuePtrVector &func,
                             const LispExecutionContext &execution_context,
                             const std::any &context_param) {
    ensure_params(func);

    LispValuePtrVector params = extract_params(func);

    params = execute_functions_and_extract_list_results(
        params, execution_context, context_param);

    params.erase(params.begin());

    return LispValueFactory::new_list(params);
  }
};

#endif
