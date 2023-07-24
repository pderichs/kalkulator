#include <algorithm>
#include <any>
#include <memory>

#include "lisp_function.h"
#include "lisp_tokens.h"

#include "lisp_function_error.h"
#include "lisp_value.h"

LispFunction::LispFunction(const LispTokens &tokens) {
  LispTokens::const_iterator tokens_it = tokens.begin();
  _tokens = read_function_param_tokens(tokens_it);

  // Parse identifier

  // Find first open bracket token
  LispTokens::const_iterator it =
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

void LispFunction::read_params(LispTokens::const_iterator it) {
  _params.clear();

  bool end_bracket_reached = false;
  int bracket_level = 1;

  while (it != _tokens.end() && !end_bracket_reached) {
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
      case OPEN_BRACKET: {
        bracket_level++;
        auto func_param_tokens = read_function_param_tokens(it);
        LispFunction func(func_param_tokens);
        _params.push_back(std::make_shared<LispValue>(func));
      } break;
      case CLOSE_BRACKET:
        bracket_level--;
        end_bracket_reached = bracket_level == 0;
        break;
      default:
        throw LispFunctionError("Syntax error.");
      }
    } catch (std::bad_any_cast &bac) {
      throw LispFunctionError(bac.what());
    }

    it++;
  }

  if (!end_bracket_reached) {
    throw LispFunctionError("Syntax error: closing bracket is missing.");
  }
}

void LispFunction::overread_spaces(LispTokens::const_iterator &it) const {
  while (true) {
    const auto &token = *it;

    if (token.id != SPACE) {
      break;
    }

    it++;
  }
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

LispTokens
LispFunction::read_function_param_tokens(LispTokens::const_iterator &it) const {
  LispTokens result;

  int bracket_level = 0;

  while (true) {
    auto token = *it;

    if (token.is_open_bracket()) {
      bracket_level++;
      result.push_back(token);
    } else if (token.is_closed_bracket()) {
      bracket_level--;
      result.push_back(token);

      if (bracket_level == 0) {
        break;
      }
    } else {
      result.push_back(token);
    }

    it++;
  }

  return result;
}
