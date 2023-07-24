#include "lisp_value_parser.h"
#include "lisp_function.h"
#include "lisp_parser_error.h"
#include "lisp_tokens.h"
#include "lisp_value.h"
#include <any>
#include <memory>
#include <sstream>

std::optional<LispValue> LispValueParser::next() {
  if (!has_next()) {
    return {};
  }

  skip_spaces();

  LispToken token = current_token();

  if (token.is_number()) {
    double d = std::any_cast<double>(token.content);
    _pos++;
    return LispValue(d);
  } else if (token.is_string()) {
    std::string s = std::any_cast<std::string>(token.content);
    _pos++;
    return LispValue(s);
  }

  // Check for function
  if (token.is_open_bracket()) {
    return parse_function();
  }

  return {};
}

void LispValueParser::skip_spaces() {
  while (has_next()) {
    if (!current_token().is_space()) {
      break;
    }

    _pos++;
  }
}

LispValue LispValueParser::parse_function() {
  LispTokens function_tokens = collect_current_function_tokens();

  bool identifier_search = false;
  bool first_bracket_found = false;
  bool in_params = false;

  std::string identifier;

  LispTokens param_tokens;

  for (auto token : function_tokens) {
    if (token.is_space()) {
      continue;
    }

    if (!first_bracket_found) {
      if (token.is_open_bracket()) {
        first_bracket_found = true;
        identifier_search = true;
        continue;
      } else {
        throw LispParserError("Unexpected token");
      }
    }

    if (identifier_search) {
      if (token.is_identifier()) {
        identifier_search = false;
        in_params = true;

        identifier = std::any_cast<std::string>(token.content);

        continue;
      } else {
        std::stringstream ss;
        ss << "Expected function identifier but got type ";
        ss << token.id;
        throw LispParserError(ss.str());
      }
    }

    if (in_params) {
      param_tokens.push_back(token);
    }
  }

  LispValueParser params_parser(param_tokens);
  std::vector<std::shared_ptr<LispValue>> params;

  while (true) {
    auto next = params_parser.next();
    if (!next) {
      break;
    }

    auto value = *next;
    params.push_back(std::make_shared<LispValue>(value));
  }

  return LispValue(LispFunction(identifier, params));
}

LispTokens LispValueParser::collect_current_function_tokens() {
  LispTokens result;

  int bracket_level = 0;

  while (true) {
    LispToken token = current_token();

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

    _pos++;
  }

  return result;
}
