#include <algorithm>

#include "lisp_function.h"
#include "lisp_tokens.h"

LispFunction::LispFunction(const LispTokens& tokens) {
  _tokens = tokens;

  // Find first open bracket token
  auto it = std::find_if(_tokens.begin(), _tokens.end(), [](const LispToken& token) {
    return token.id == OPEN_BRACKET;
  });

  if (it == _tokens.end()) {
    // TODO: Error!
  }

  it++;

  parse_function(it);
}

void LispFunction::parse_function(LispTokens::iterator it) {
  const auto& func_token = *it;
  if (func_token.id != IDENTIFIER) {
    // TODO: Error!
  }
}
