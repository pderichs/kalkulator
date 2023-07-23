#include <algorithm>
#include <any>
#include <memory>

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

  it++;

  // Parameters
  read_params(it);
}

void LispFunction::read_params(LispTokens::iterator it) {
  _params.clear();

  bool end_bracket_reached = false;
  int bracket_level = 0;

  while (!end_bracket_reached) {
    overread_spaces(it);

    const auto &token = *it;
    double d;
    std::string s;

    try {
      switch (token.id) {
      case NUMBER:
        d = std::any_cast<double>(token.content);
        _params.push_back(std::make_shared<LispValue>(d));
        break;
      case STRING:
        s = std::any_cast<std::string>(token.content);
        _params.push_back(std::make_shared<LispValue>(s));
        break;
      case OPEN_BRACKET:
        bracket_level++;
        // TODO: Parse function
        break;

      case CLOSE_BRACKET:
        bracket_level--;
        end_bracket_reached = bracket_level < 0;
        break;
      default:
        throw LispFunctionError("Syntax error.");
      }
    } catch (std::bad_any_cast &bac) {
      throw LispFunctionError(bac.what());
    }

    it++;
  }
}

void LispFunction::overread_spaces(LispTokens::iterator &it) const {
  while (true) {
    const auto &token = *it;

    if (token.id != SPACE) {
      break;
    }

    it++;
  }
}

std::optional<std::shared_ptr<LispValue>> LispFunction::param_at(size_t t) const {
  std::optional<std::shared_ptr<LispValue>> result;

  if (t >= _params.size()) {
    return {};
  }

  result = _params[t];

  return result;
}
