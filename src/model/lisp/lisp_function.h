#ifndef LISP_FUNCTION_INCLUDED
#define LISP_FUNCTION_INCLUDED

#include <cstddef>
#include <memory>
#include <string>

#include "lisp_tokens.h"

class LispValue;

class LispFunction {
private:
  LispTokens _tokens;
  std::string _identifier;
  std::vector<std::shared_ptr<LispValue>> _params;

public:
  LispFunction(const LispTokens &tokens);

  std::string identifier() const { return _identifier; }
  size_t param_count() const { return _params.size(); }
  void parse_function(LispTokens::iterator it);
};

#endif
