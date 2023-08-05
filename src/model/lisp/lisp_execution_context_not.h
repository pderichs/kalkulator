#ifndef LISP_EXECUTION_CONTEXT_NOT_INCLUDED
#define LISP_EXECUTION_CONTEXT_NOT_INCLUDED

#include "lisp_execution_context.h"
#include "lisp_execution_context_error.h"
#include "lisp_function.h"
#include "lisp_value.h"

class LispExecutionContextNot : public LispFunctionExecutionContext {
public:
  LispExecutionContextNot() = default;
  virtual ~LispExecutionContextNot() = default;

  virtual LispValue value(const LispFunction &func,
                          const LispExecutionContext &execution_context,
                          const std::any &context_param) {
    if (func.param_count() < 1) {
      throw LispExecutionContextError("Expected one parameter for not");
    }

    LispValuePtrVector params = execute_functions_and_extract_list_results(
        func.params(), execution_context, context_param);

    for (const auto &param : params) {
      if (param->is_truthy()) {
        return LispValue(LISP_BOOL_FALSE);
      }
    }

    return LispValue(LISP_BOOL_TRUE);
  }
};
#endif
