#ifndef LISP_EXECUTOR_INCLUDED
#define LISP_EXECUTOR_INCLUDED

#include "lisp_function.h"
#include "lisp_value.h"
class LispExecutionContext {
private:
  LispValue _value;

public:
  LispExecutionContext(const LispValue &value);
  LispValue execute() const;

private:
  LispValue eval_function() const;

  LispValue add(const LispFunction& func) const;
  LispValue subtract(const LispFunction& func) const;
  LispValue multiply(const LispFunction& func) const;
  LispValue divide(const LispFunction& func) const;
};

#endif
