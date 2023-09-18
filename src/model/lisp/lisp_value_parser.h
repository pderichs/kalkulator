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

#ifndef LISP_FUNCTION_PARSER_INCLUDED
#define LISP_FUNCTION_PARSER_INCLUDED

#include "lisp_tokens.h"
#include "lisp_value.h"
#include <cstddef>
#include <optional>

class LispValueParser {
public:
  explicit LispValueParser(const LispTokens &tokens, size_t start = 0)
      : _pos(start), _tokens(tokens) {}

  LispValuePtr next();

private:
  [[nodiscard]] bool has_next() const { return _pos < _tokens.size(); }
  [[nodiscard]] LispToken current_token() const { return _tokens[_pos]; }
  void skip_spaces();
  LispValuePtr parse_list();
  LispTokens collect_current_function_tokens();

private:
  size_t _pos;
  LispTokens _tokens;
};

#endif
