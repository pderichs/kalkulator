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

#ifndef LISP_TOKENS_INCLUDED
#define LISP_TOKENS_INCLUDED

#include <any>
#include <ostream>
#include <vector>

/**
 * Each token has a specific id. The tokenizer implements the logic to
 * identify these.
 */
enum TokenID {
  OPEN_BRACKET,
  CLOSE_BRACKET,
  IDENTIFIER,
  STRING,
  DOUBLE,
  INTEGER,
  SPACE
};

/**
 * Combines a token id with the actual content from the string.
 */
struct LispToken {
  TokenID id;
  std::any content;

  bool is_space() const { return id == SPACE; }
  bool is_identifier() const { return id == IDENTIFIER; }
  bool is_string() const { return id == STRING; }
  bool is_integer() const { return id == INTEGER; }
  bool is_double() const { return id == DOUBLE; }
  bool is_open_bracket() const { return id == OPEN_BRACKET; }
  bool is_closed_bracket() const { return id == CLOSE_BRACKET; }
};

typedef std::vector<LispToken> LispTokens;

#endif
