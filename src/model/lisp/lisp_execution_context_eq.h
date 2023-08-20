#ifndef LISP_EXECUTION_CONTEXT_EQ_INCLUDED
#define LISP_EXECUTION_CONTEXT_EQ_INCLUDED

#include "lisp_execution_context.h"
#include "lisp_function.h"
#include "lisp_value.h"

class LispExecutionContextEq : public LispFunctionExecutionContext {
public:
  LispExecutionContextEq(bool extract_lists) { _extract_lists = extract_lists; }
  virtual ~LispExecutionContextEq() = default;

  virtual LispValuePtr value(const LispFunction &func,
                          const LispExecutionContext &execution_context,
                          const std::any &context_param) {
    ensure_params(func);

    LispValuePtrVector params;

    if (_extract_lists) {
      params = execute_functions_and_extract_list_results(
          func.params(), execution_context, context_param);
    } else {
      params =
          execute_functions(func.params(), execution_context, context_param);
    }

    LispValuePtr last;
    for (const auto &param : params) {
      if (last && !last->content_equals(*param)) {
        return LispValueFactory::new_bool(LISP_BOOL_FALSE);
      }

      last = param;
    }

    return LispValueFactory::new_bool(LISP_BOOL_TRUE);
  }

private:
  bool _extract_lists;
};

#endif
