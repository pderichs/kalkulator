#include "lisp_value_parser.h"
#include "lisp_tokens.h"
#include "lisp_value.h"
#include <any>

std::optional<LispValue> LispValueParser::next() {
  if (!has_next()) {
    return {};
  }

  skip_spaces();

  LispToken token = current_token();

  if (token.is_number()) {
    double d = std::any_cast<double>(token.content);
    return LispValue(d);
  } else if (token.is_string()) {
    std::string s = std::any_cast<std::string>(token.content);
    return LispValue(s);
  }

  // Check for function
  // TODO

}

void LispValueParser::skip_spaces() {
  while (has_next()) {
    if (!current_token().is_space()) {
      break;
    }

    _pos++;
  }
}
