#ifndef LISP_EXPRESSION_INCLUDED
#define LISP_EXPRESSION_INCLUDED

#include <cstddef>
#include <string>

#include "lisp_tokens.h"
#include "lisp_value.h"

class LispFunction {
private:
  LispTokens _tokens;
  std::string _identifier;
  LispValueVector _params;

public:
  LispFunction(const LispTokens &tokens);

  std::string identifier() const { return _identifier; }
  size_t param_count() const { return _params.size(); }
  void parse_function(LispTokens::iterator it);
};

#endif
