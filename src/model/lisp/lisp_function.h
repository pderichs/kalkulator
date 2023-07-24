#ifndef LISP_FUNCTION_INCLUDED
#define LISP_FUNCTION_INCLUDED

#include <cstddef>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "lisp_tokens.h"

class LispValue;

typedef std::shared_ptr<LispValue> LispValuePtr;
typedef std::vector<LispValuePtr> LispValuePtrVector;

class LispFunction {
private:
  LispTokens _tokens;
  std::string _identifier;
  LispValuePtrVector _params;

public:
  LispFunction(const std::string& identifier, const LispValuePtrVector& params);

  std::string identifier() const { return _identifier; }
  size_t param_count() const { return _params.size(); }

  std::optional<LispValuePtr> param_at(size_t t) const;
};

#endif
