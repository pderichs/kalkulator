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

#ifndef LISP_SYNTAX_CHECKER_INCLUDED
#define LISP_SYNTAX_CHECKER_INCLUDED

#include "lisp_tokens.h"

/**
 * Performs a few additional syntax checks based on a set of tokens.
 */
class LispSyntaxChecker {
public:
  explicit LispSyntaxChecker(const LispTokens &tokens);
  void check() const;

private:
  void check_matching_brackets() const;
  void check_valid_functions() const;

private:
  LispTokens _tokens;
};

#endif
