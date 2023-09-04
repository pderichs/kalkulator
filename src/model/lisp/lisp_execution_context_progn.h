#ifndef EXECUTION_CONTEXT_PROGN_INCLUDED
#define EXECUTION_CONTEXT_PROGN_INCLUDED

#include "lisp_function.h"
#include "lisp_function_execution_context.h"
#include "lisp_value_factory.h"

class LispExecutionContextProgn : public LispFunctionExecutionContext {
public:
  LispExecutionContextProgn() = default;
  virtual ~LispExecutionContextProgn() = default;

  virtual LispValuePtr value(const LispFunction &func,
                             const LispExecutionContext &execution_context,
                             const std::any &context_param) {
    ensure_params(func);

    LispValuePtrVector params = execute_functions_and_extract_list_results(
        func.params(), execution_context, context_param);

    // Only the last result is relevant for progn
    return params[params.size() - 1];
  }
};

#endif
