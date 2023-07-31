#ifndef LISP_EXECUTION_CONTEXT_FIRST_INCLUDED
#define LISP_EXECUTION_CONTEXT_FIRST_INCLUDED

#include "lisp_function_execution_context.h"

class LispExecutionContextFirst : public LispFunctionExecutionContext {
public:
  LispExecutionContextFirst() = default;
  virtual ~LispExecutionContextFirst() = default;

  virtual LispValue value(const LispFunction &func,
                          const LispExecutionContext &execution_context) {
    ensure_params(func);

    // TODO: Should we execute functions here or return the function
    // value instead?
    LispValuePtrVector params = execute_functions_and_extract_list_results(
        func.params(), execution_context);

    return LispValue(*params[0]);
  }
};


#endif
