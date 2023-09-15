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


#include "lisp_tokens.h"
#include "lisp_value.h"

#include <sstream>

void LispTokens::debug_print(std::ostream &oss) const {
  std::string s;

  int n = 1;
  for (auto token : *this) {
    s = token_id_to_string(token.id);
    oss << n << ".) " << s << ", content: " << printable_content_of_token(token)
        << std::endl;
    n++;
  }
}

std::string token_id_to_string(TokenID token_id) {
  switch (token_id) {
  case IDENTIFIER:
    return "identifier";
  case STRING:
    return "string";
  case INTEGER:
    return "integer";
  case DOUBLE:
    return "double";
  case OPEN_BRACKET:
    return "open bracket";
  case CLOSE_BRACKET:
    return "closed bracket";
  case SPACE:
    return "space";
  default:
    return "(other token)";
  }
}

std::string printable_content_of_token(const LispToken &token) {
  std::string s;
  LispValue::DoubleType d;

  switch (token.id) {
  case IDENTIFIER:
  case STRING:
    s = std::any_cast<std::string>(token.content);
    return s;
  case DOUBLE: {
    d = std::any_cast<LispValue::DoubleType>(token.content);

    std::stringstream ss;
    ss << d;
    return ss.str();
  }
  case INTEGER: {
    d = std::any_cast<LispValue::IntegerType>(token.content);

    std::stringstream ss;
    ss << d;
    return ss.str();
  }
  case OPEN_BRACKET:
    return "(";
  case CLOSE_BRACKET:
    return ")";
  case SPACE:
    return "";
  default:
    return "(other token content)";
  }
}
