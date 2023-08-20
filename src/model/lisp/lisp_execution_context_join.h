#ifndef LISP_EXECUTION_CONTEXT_JOIN_INCLUDED
#define LISP_EXECUTION_CONTEXT_JOIN_INCLUDED

#include "lisp_function.h"
#include "lisp_function_execution_context.h"
#include "lisp_value_factory.h"

class LispExecutionContextJoin : public LispFunctionExecutionContext {
public:
  LispExecutionContextJoin() = default;
  virtual ~LispExecutionContextJoin() = default;

  virtual LispValuePtr value(const LispFunction &func,
                          const LispExecutionContext &execution_context,
                          const std::any &context_param) {
    LispValuePtrVector result;

    LispValuePtrVector params = execute_functions_and_extract_list_results(
        func.params(), execution_context, context_param);

    for (const auto &param : params) {
        result.push_back(param);
    }

    return LispValueFactory::new_list(result);
  }
};

#endif
