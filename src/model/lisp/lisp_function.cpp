#include <algorithm>
#include <any>

#include "lisp_function.h"
#include "lisp_tokens.h"

#include "lisp_function_error.h"
#include "lisp_value.h"

LispFunction::LispFunction(const LispTokens &tokens) {
  _tokens = tokens;

  // Parse identifier

  // Find first open bracket token
  auto it =
      std::find_if(_tokens.begin(), _tokens.end(), [](const LispToken &token) {
        return token.id == OPEN_BRACKET;
      });

  if (it == _tokens.end()) {
    throw LispFunctionError("Unable to find start of function expression.");
  }

  it++;

  if (it == _tokens.end()) {
    throw LispFunctionError("Malformed expression.");
  }

  const auto &token = *it;

  if (token.id != IDENTIFIER) {
    throw LispFunctionError(
        "Malformed function - identifier has wrong format.");
  }

  try {
    _identifier = std::any_cast<std::string>(token.content);
  } catch (std::bad_any_cast &bad_cast_exception) {
    throw LispFunctionError(bad_cast_exception.what());
  }

  // Parameters


  parse_function(it);
}

void LispFunction::parse_function(LispTokens::iterator it) {
  const auto &func_token = *it;
  if (func_token.id != IDENTIFIER) {
    // TODO: Error!
  }
}
