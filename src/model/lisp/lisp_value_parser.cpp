/**
 * Kalkulator - a simple and small spread sheet app.
 * Copyright (C) 2023  pderichs (derichs@posteo.de)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "lisp_value_parser.h"
#include "lisp_parser_error.h"
#include "lisp_tokens.h"
#include "lisp_value.h"
#include "lisp_value_factory.h"
#include <any>
#include <memory>

LispValuePtr LispValueParser::next() {
  if (!has_next()) {
    return {};
  }

  skip_spaces();

  LispToken token = current_token();

  if (token.is_double()) {
    auto d =
        std::any_cast<LispValue::DoubleType>(token.content);
    _pos++;
    return LispValueFactory::new_double(d);
  } else if (token.is_integer()) {
    auto d =
        std::any_cast<LispValue::IntegerType>(token.content);
    _pos++;
    return LispValueFactory::new_integer(d);
  } else if (token.is_string()) {
    auto s = std::any_cast<std::string>(token.content);
    _pos++;
    return LispValueFactory::new_string(s);
  } else if (token.is_identifier()) {
    // Same as string but with identifier flag set to true
    auto s = std::any_cast<std::string>(token.content);
    _pos++;
    return LispValueFactory::new_identifier(s);
  }

  // Check for function
  if (token.is_open_bracket()) {
    return parse_list();
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

LispValuePtr LispValueParser::parse_list() {
  LispTokens function_tokens = collect_current_function_tokens();

  // std::cerr << "Function tokens:" << std::endl;
  // function_tokens.debug_print(std::cerr);

  bool first_bracket_found = false;
  bool in_params = false;

  int in_param_bracket_level = 0;

  LispTokens param_tokens;

  for (const auto &token : function_tokens) {
    if (token.is_space()) {
      continue;
    }

    if (!first_bracket_found) {
      if (token.is_open_bracket()) {
        first_bracket_found = true;
        in_params = true;
        in_param_bracket_level = 0;
        continue;
      } else {
        throw LispParserError("Unexpected token");
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

  return LispValueFactory::new_list(params);
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
