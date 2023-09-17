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


#include "lisp_syntax_checker.h"
#include "lisp_parser_error.h"
#include <sstream>

LispSyntaxChecker::LispSyntaxChecker(const LispTokens &tokens): _tokens(tokens) {}

void LispSyntaxChecker::check() const {
  check_matching_brackets();
  check_valid_functions();
}

void LispSyntaxChecker::check_matching_brackets() const {
  int open_brackets = 0;
  int closed_brackets = 0;

  bool expecting_open_bracket = true;

  for (const auto &token : _tokens) {
    if (token.is_open_bracket()) {
      expecting_open_bracket = false;
      open_brackets++;
    } else if (token.is_closed_bracket()) {
      if (expecting_open_bracket) {
        throw LispParserError("Unexpected closing bracket");
      }

      closed_brackets++;

      if (closed_brackets > open_brackets) {
        throw LispParserError("Unexpected closing bracket (2)");
      }
    }
  }

  if (open_brackets != closed_brackets) {
    std::stringstream ss;
    ss << "Brackets count mismatch: " << open_brackets << "/"
       << closed_brackets;
    throw LispParserError(ss.str());
  }

  if (open_brackets == 0 || closed_brackets == 0) {
    throw LispParserError("Not a lisp expression");
  }
}

void LispSyntaxChecker::check_valid_functions() const {
  bool next_identifier = false;

  for (const auto &token : _tokens) {
    if (token.is_open_bracket()) {
      next_identifier = true;
    } else if (next_identifier) {
      if (!token.is_identifier()) {
        throw LispParserError("Expected function call identifier");
      }

      next_identifier = false;
    }
  }
}
