#ifndef LISP_EXECUTION_CONTEXT_CONS_INCLUDED
#define LISP_EXECUTION_CONTEXT_CONS_INCLUDED

#include "lisp_execution_context_error.h"
#include "lisp_function.h"
#include "lisp_value_factory.h"
#include <tuple>

class LispExecutionContextCons : public LispFunctionExecutionContext {
public:
  LispExecutionContextCons() = default;
  virtual ~LispExecutionContextCons() = default;

  virtual LispValuePtr value(const LispFunction &func,
                             const LispExecutionContext &execution_context,
                             const std::any &context_param) {
    std::ignore = execution_context;
    std::ignore = context_param;

    if (func.param_count() != 2) {
      throw LispExecutionContextError("cons expects two parameters");
    }

    LispValuePtrVector result;

    for (const auto &param : func.params()) {
      result.push_back(param);
    }

    return LispValueFactory::new_list(result);
  }
};

#endif
