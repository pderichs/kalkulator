#ifndef LISP_EXECUTOR_INCLUDED
#define LISP_EXECUTOR_INCLUDED

#include "lisp_value.h"
class LispExecutor {
private:
  LispValue _value;

public:
  LispExecutor(const LispValue &value);
  LispValue execute() const;
};

#endif
