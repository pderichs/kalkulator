#ifndef LISP_EXECUTION_CONTEXT_REST_INCLUDED
#define LISP_EXECUTION_CONTEXT_REST_INCLUDED

#include "lisp_function.h"
#include "lisp_function_execution_context.h"
#include "lisp_value_factory.h"

class LispExecutionContextRest : public LispFunctionExecutionContext {
public:
  LispExecutionContextRest() = default;
  virtual ~LispExecutionContextRest() = default;

  virtual LispValuePtr value(const LispFunction &func,
                          const LispExecutionContext &execution_context,
                          const std::any &context_param) {
    ensure_params(func);

    // TODO: Should we execute functions here or return the function
    // value instead?
    LispValuePtrVector params = execute_functions_and_extract_list_results(
        func.params(), execution_context, context_param);

    params.erase(params.begin());

    return LispValueFactory::new_list(params);
  }
};

#endif
