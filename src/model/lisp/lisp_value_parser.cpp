#include "lisp_value_parser.h"
#include "lisp_function.h"
#include "lisp_parser_error.h"
#include "lisp_tokens.h"
#include "lisp_value.h"
#include <any>
#include <iostream>
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
  } else if (token.is_identifier()) {
    // Same as string but with identifier flag set to true
    std::string s = std::any_cast<std::string>(token.content);
    _pos++;
    return LispValue(s, true);
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

  // std::cerr << "Function tokens:" << std::endl;
  // function_tokens.debug_print(std::cerr);

  bool identifier_search = false;
  bool first_bracket_found = false;
  bool in_params = false;

  int in_param_bracket_level = 0;

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
        in_param_bracket_level = 0;

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
      if (token.is_open_bracket()) {
        in_param_bracket_level++;
      }

      if (token.is_closed_bracket()) {
        in_param_bracket_level--;

        if (in_param_bracket_level < 0) {
          break;
        }
      }

      param_tokens.push_back(token);
    }
  }

  // std::cerr << "Param tokens:" << std::endl;
  // param_tokens.debug_print(std::cerr);

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

  while (has_next()) {
    LispToken token = current_token();

    if (token.is_open_bracket()) {
      bracket_level++;
      result.push_back(token);
    } else if (token.is_closed_bracket()) {
      bracket_level--;
      result.push_back(token);
      if (bracket_level == 0) {
        _pos++;
        break;
      }
    } else {
      result.push_back(token);
    }

    _pos++;
  }

  return result;
}
