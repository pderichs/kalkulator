#ifndef LISP_EXECUTOR_INCLUDED
#define LISP_EXECUTOR_INCLUDED

#include <map>

#include "lisp_function.h"
#include "lisp_function_execution_context.h"
#include "lisp_value.h"

typedef std::map<std::string, LispFunctionExecutionContextPtr> LispFunctionMap;

class LispExecutionContext {
private:
  LispValue _value;
  LispFunctionMap _functions;

public:
  LispExecutionContext(const LispValue &value);
  LispValue execute() const;
  void add_function(const std::string &identifier,
                    const LispFunctionExecutionContextPtr func);

private:
  LispValue eval_function() const;
};

#endif
