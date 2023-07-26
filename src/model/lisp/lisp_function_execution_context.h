#ifndef LISP_FUNCTION_EXECUTION_CONTEXT_INCLUDED
#define LISP_FUNCTION_EXECUTION_CONTEXT_INCLUDED

#include "lisp_function.h"
#include "lisp_value.h"
#include <memory>

class LispFunctionExecutionContext {
public:
    virtual LispValue value(const LispFunction& func) = 0;
};

typedef std::shared_ptr<LispFunctionExecutionContext> LispFunctionExecutionContextPtr;

#endif
