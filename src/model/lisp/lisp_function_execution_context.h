#ifndef LISP_FUNCTION_EXECUTION_CONTEXT_INCLUDED
#define LISP_FUNCTION_EXECUTION_CONTEXT_INCLUDED

#include <memory>

#include "lisp_execution_context_error.h"
#include "lisp_function.h"
#include "lisp_value.h"

class LispFunctionExecutionContext {
public:
  virtual LispValue value(const LispFunction &func) = 0;

  void ensure_params(const LispFunction &func) const;
  LispValue execute_function(const LispFunction &func) const;
};

typedef std::shared_ptr<LispFunctionExecutionContext>
    LispFunctionExecutionContextPtr;

#endif
