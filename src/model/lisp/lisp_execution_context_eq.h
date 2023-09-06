#ifndef LISP_EXECUTION_CONTEXT_EQ_INCLUDED
#define LISP_EXECUTION_CONTEXT_EQ_INCLUDED

#include "lisp_execution_context.h"
#include "lisp_value.h"

class LispExecutionContextEq : public LispFunctionExecutionContext {
public:
  LispExecutionContextEq(bool extract_lists) { _extract_lists = extract_lists; }
  virtual ~LispExecutionContextEq() = default;

  virtual LispValuePtr value(const LispValuePtrVector &func,
                             const LispExecutionContext &execution_context,
                             const std::any &context_param) {
    ensure_params(func);

    LispValuePtrVector params = extract_params(func);

    if (_extract_lists) {
      params = execute_functions_and_extract_list_results(
          params, execution_context, context_param);
    } else {
      params =
          execute_functions(params, execution_context, context_param);
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
