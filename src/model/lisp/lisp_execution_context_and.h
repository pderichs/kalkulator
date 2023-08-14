#ifndef LISP_EXECUTION_CONTEXT_AND_INCLUDED
#define LISP_EXECUTION_CONTEXT_AND_INCLUDED

#include "lisp_execution_context.h"
#include "lisp_function.h"
#include "lisp_value.h"

class LispExecutionContextAnd : public LispFunctionExecutionContext {
public:
  LispExecutionContextAnd() = default;
  virtual ~LispExecutionContextAnd() = default;

  virtual LispValue value(const LispFunction &func,
                          const LispExecutionContext &execution_context,
                          const std::any &context_param) {
    ensure_params(func);

    LispValuePtrVector params = execute_functions_and_extract_list_results(
        func.params(), execution_context, context_param);

    for (const auto &param : params) {
      if (!param->is_truthy()) {
        return LispValue(LISP_BOOL_FALSE);
      }
    }

    return LispValue(LISP_BOOL_TRUE);
  }
};

#endif
