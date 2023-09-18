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


#ifndef LISP_PARSER_ERROR_INCLUDED
#define LISP_PARSER_ERROR_INCLUDED

#include <stdexcept>

class LispParserError : public std::runtime_error {
public:
  explicit LispParserError(const std::string &msg) : std::runtime_error(msg), _item() {}
  LispParserError(const std::string &msg, const std::string &item)
      : std::runtime_error(msg), _item(item) {}

  [[nodiscard]] std::string item() const { return _item; }

private:
  std::string _item;
};

#endif
