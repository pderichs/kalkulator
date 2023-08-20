#ifndef LISP_EXECUTION_CONTEXT_NOT_INCLUDED
#define LISP_EXECUTION_CONTEXT_NOT_INCLUDED

#include "lisp_execution_context.h"
#include "lisp_execution_context_error.h"
#include "lisp_function.h"
#include "lisp_value.h"
#include "lisp_value_factory.h"

class LispExecutionContextNot : public LispFunctionExecutionContext {
public:
  LispExecutionContextNot() = default;
  virtual ~LispExecutionContextNot() = default;

  virtual LispValuePtr value(const LispFunction &func,
                          const LispExecutionContext &execution_context,
                          const std::any &context_param) {
    ensure_params(func);

    LispValuePtrVector params = execute_functions_and_extract_list_results(
        func.params(), execution_context, context_param);

    for (const auto &param : params) {
      if (param->is_truthy()) {
        return LispValueFactory::new_bool(LISP_BOOL_FALSE);
      }
    }

    return LispValueFactory::new_bool(LISP_BOOL_TRUE);
  }
};

#endif
