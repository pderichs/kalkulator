#ifndef LISP_FUNCTION_INCLUDED
#define LISP_FUNCTION_INCLUDED

#include <cstddef>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "lisp_tokens.h"

typedef std::vector<LispTokens> LispTokensVector;

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

  std::optional<std::shared_ptr<LispValue>> param_at(size_t t) const;

private:
  void read_params(LispTokens::iterator it);
  void overread_spaces(LispTokens::iterator &it) const;
};

#endif
