#include <any>
#include <memory>

#include "lisp_function.h"
#include "lisp_tokens.h"

#include "lisp_value.h"

LispFunction::LispFunction(const std::string &identifier,
                           const LispValuePtrVector &params) {
  _identifier = identifier;
  _params = params;
}

std::optional<std::shared_ptr<LispValue>>
LispFunction::param_at(size_t t) const {
  std::optional<std::shared_ptr<LispValue>> result;

  if (t >= _params.size()) {
    return {};
  }

  result = _params[t];

  return result;
}
