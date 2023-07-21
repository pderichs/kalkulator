#ifndef LISP_RUNNER_INCLUDED
#define LISP_RUNNER_INCLUDED

#include "lisp_function.h"
#include "lisp_tokens.h"
#include "lisp_value.h"

class LispRunner {
private:
  LispFunction _function;

public:
  LispRunner(const LispFunction &function);
  ~LispRunner() = default;

  LispValue run() const;
};

#endif
