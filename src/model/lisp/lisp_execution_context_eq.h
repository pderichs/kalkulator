#ifndef LISP_EXECUTION_CONTEXT_EQ_INCLUDED
#define LISP_EXECUTION_CONTEXT_EQ_INCLUDED

#include "lisp_execution_context.h"
#include "lisp_function.h"
#include "lisp_value.h"

class LispExecutionContextEq : public LispFunctionExecutionContext {
public:
  LispExecutionContextEq() = default;
  virtual ~LispExecutionContextEq() = default;

  virtual LispValue value(const LispFunction &func,
                          const LispExecutionContext &execution_context,
                          const std::any &context_param) {
    ensure_params(func);

    // TODO: Problem / to verify: We would never be able to give 2 lists into
    // that function and check for their equality, since the funcion call to
    // "list" would extract the items right here. The eq function with the
    // extraction is nevertheless useful since it is e.g. simplifying the
    // handling of cell_range equality checks.
    // Possible solution: Create two separate equality functions: one which
    // extracts lists and one which doesn't.
    LispValuePtrVector params = execute_functions_and_extract_list_results(
        func.params(), execution_context, context_param);

    LispValuePtr last;
    for (const auto &param : params) {
      if (last) {
        LispValue eq_result = last->content_equals(*param);
        if (!eq_result.is_truthy()) {
          return LispValue(LISP_BOOL_FALSE);
        }
      }

      last = param;
    }

    return LispValue(LISP_BOOL_TRUE);
  }
};

#endif
