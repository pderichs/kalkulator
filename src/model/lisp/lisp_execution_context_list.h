#ifndef LISP_EXECUTION_CONTEXT_LIST_INCLUDED
#define LISP_EXECUTION_CONTEXT_LIST_INCLUDED

#include "lisp_function.h"
#include "lisp_function_execution_context.h"
#include "lisp_value_factory.h"
#include <tuple>

class LispExecutionContextList : public LispFunctionExecutionContext {
public:
  LispExecutionContextList() = default;
  virtual ~LispExecutionContextList() = default;

  virtual LispValuePtr value(const LispFunction &func,
                          const LispExecutionContext &execution_context,
                          const std::any &context_param) {
    std::ignore = execution_context;
    std::ignore = context_param;

    return LispValueFactory::new_list(func.params()); // ;)
  }
};

#endif
