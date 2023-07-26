#ifndef LISP_FUNCTION_EXECUTION_CONTEXT_INCLUDED
#define LISP_FUNCTION_EXECUTION_CONTEXT_INCLUDED

#include "lisp_execution_context_error.h"
#include "lisp_function.h"
#include "lisp_value.h"
#include <memory>

class LispFunctionExecutionContext {
public:
    virtual LispValue value(const LispFunction& func) = 0;

    void ensure_params(const LispFunction& func) {
      if (func.param_count() == 0) {
        throw LispExecutionContextError("Unexpected: No params given");
      }
    }
};

typedef std::shared_ptr<LispFunctionExecutionContext> LispFunctionExecutionContextPtr;

#endif
