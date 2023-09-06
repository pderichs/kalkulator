#ifndef LISP_EXECUTION_CONTEXT_AND_INCLUDED
#define LISP_EXECUTION_CONTEXT_AND_INCLUDED

#include "lisp_execution_context.h"
#include "lisp_value.h"

class LispExecutionContextAnd : public LispFunctionExecutionContext {
public:
  LispExecutionContextAnd() = default;
  virtual ~LispExecutionContextAnd() = default;

  virtual LispValuePtr value(const LispValuePtrVector &func,
                             const LispExecutionContext &execution_context,
                             const std::any &context_param) {
    ensure_params(func);

    LispValuePtrVector params = extract_params(func);

    params = execute_functions_and_extract_list_results(
        params, execution_context, context_param);

    for (const auto &param : params) {
      if (!param->is_truthy()) {
        return LispValueFactory::new_bool(LISP_BOOL_FALSE);
      }
    }

    return LispValueFactory::new_bool(LISP_BOOL_TRUE);
  }
};

#endif
