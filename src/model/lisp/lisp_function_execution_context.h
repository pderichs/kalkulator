#ifndef LISP_FUNCTION_EXECUTION_CONTEXT_INCLUDED
#define LISP_FUNCTION_EXECUTION_CONTEXT_INCLUDED

#include <memory>

#include "lisp_execution_context_error.h"
#include "lisp_function.h"
#include "lisp_value.h"
#include "lisp_execution_context.h"


class LispFunctionExecutionContext {
public:
  virtual LispValue value(const LispFunction &func, const LispExecutionContext& execution_context) = 0;

  void ensure_params(const LispFunction &func) const;
};

#endif
