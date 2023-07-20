#ifndef LISP_RUNNER_INCLUDED
#define LISP_RUNNER_INCLUDED

#include "lisp_tokens.h"
#include "lisp_value.h"

class LispRunner {
private:
  LispTokens _tokens;
public:
    LispRunner(const LispTokens& tokens);
    ~LispRunner() = default;

    LispValue run() const;
};

#endif
