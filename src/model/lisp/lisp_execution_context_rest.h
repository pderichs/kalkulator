#ifndef LISP_EXECUTION_CONTEXT_REST_INCLUDED
#define LISP_EXECUTION_CONTEXT_REST_INCLUDED

#include "lisp_function.h"
#include "lisp_function_execution_context.h"

class LispExecutionContextRest : public LispFunctionExecutionContext {
public:
  LispExecutionContextRest() = default;
  virtual ~LispExecutionContextRest() = default;

  virtual LispValue value(const LispFunction &func,
                          const LispExecutionContext &execution_context) {
    ensure_params(func);

    // TODO: Should we execute functions here or return the function
    // value instead?
    LispValuePtrVector params = execute_functions_and_extract_list_results(
        func.params(), execution_context);

    params.erase(params.begin());

    return LispValue(params);
  }
};


#endif