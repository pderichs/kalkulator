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

#ifndef LISP_PARSER_INCLUDED
#define LISP_PARSER_INCLUDED

#include "lisp_tokens.h"
#include "lisp_value.h"
#include <cstddef>
#include <string>

/**
 * Handles all the logic to convert a string to a set of tokens (Tokenizer).
 */
class LispParser {
public:
  explicit LispParser(const std::string &lisp);
  ~LispParser() = default;

  LispTokens parse();

private:
  LispToken create_double_token(LispValue::DoubleType d);
  LispToken create_integer_token(LispValue::IntegerType d);
  LispToken create_string_token(const std::string &s);
  LispToken create_identifier_token(const std::string &s);
  LispToken create_open_bracket_token();
  LispToken create_close_bracket_token();
  LispToken create_space_token();

  void start_parsing();
  LispToken read_string();
  LispToken read_number();
  LispToken read_identifier();
  bool walk();
  char current_char() const;

private:
  std::string _lisp;
  size_t _pos;
};

#endif
