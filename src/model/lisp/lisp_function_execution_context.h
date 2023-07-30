#ifndef LISP_FUNCTION_EXECUTION_CONTEXT_INCLUDED
#define LISP_FUNCTION_EXECUTION_CONTEXT_INCLUDED

#include <memory>

#include "lisp_execution_context.h"
#include "lisp_execution_context_error.h"
#include "lisp_function.h"
#include "lisp_value.h"

class LispFunctionExecutionContext {
public:
  LispFunctionExecutionContext() = default;
  virtual ~LispFunctionExecutionContext() = default;
  virtual LispValue value(const LispFunction &func,
                          const LispExecutionContext &execution_context) = 0;

  void ensure_params(const LispFunction &func) const;
  LispValue expect_number(const LispValuePtr &value,
                          const LispExecutionContext &execution_context) const;
};

#endif
