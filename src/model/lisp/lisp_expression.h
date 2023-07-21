#ifndef LISP_EXPRESSION_INCLUDED
#define LISP_EXPRESSION_INCLUDED

#include <string>

#include "lisp_tokens.h"
#include "lisp_value.h"

class LispExpression {
private:
  LispTokens _tokens;
  std::string _func;
  LispValueVector _params;

public:
  LispExpression(const LispTokens &tokens);
};

#endif
