#ifndef LISP_RUNNER_INCLUDED
#define LISP_RUNNER_INCLUDED

#include "lisp_expression.h"
#include "lisp_tokens.h"
#include "lisp_value.h"

class LispRunner {
private:
  LispExpression _expression;

public:
  LispRunner(const LispExpression &expression);
  ~LispRunner() = default;

  LispValue run() const;
};

#endif
