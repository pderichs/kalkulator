#ifndef LISP_FUNCTION_INCLUDED
#define LISP_FUNCTION_INCLUDED

#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "lisp_tokens.h"

class LispValue;

typedef std::shared_ptr<LispValue> LispValuePtr;
typedef std::vector<LispValuePtr> LispValuePtrVector;

class LispFunction {
private:
  LispTokens _tokens;
  std::string _identifier;
  LispValuePtrVector _params;

  // TODO: Evaluate cached result

public:
  LispFunction(const std::string &identifier, const LispValuePtrVector &params);

  LispValuePtrVector params() const { return _params; }

  std::string identifier() const { return _identifier; }
  size_t param_count() const { return _params.size(); }

  std::optional<LispValuePtr> param_at(size_t t) const;
};

#endif
