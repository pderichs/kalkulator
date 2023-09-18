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

#include "table_cell.h"

void TableCell::update_content(const std::string &content) {
  _lisp_value = ValueConverter::to_lisp_value(content);

  if (_lisp_value->is_function()) {
    _formula_content = content;
  } else {
    _formula_content = "";
  }

  recalc();
}

std::string TableCell::visible_content() const {
  return _visible_content;
}

std::string TableCell::get_formula_content() const {
  if (is_formula()) {
    return _formula_content;
  }

  return visible_content();
}

bool TableCell::is_formula() const {
  return _lisp_value && _lisp_value->is_function();
}

bool TableCell::has_content() const {
  return _lisp_value && !_lisp_value->is_none();
}

void TableCell::clear() { _lisp_value = {}; }

// Returns true if content changed, false otherwise.
bool TableCell::recalc() {
  std::string previous_content = _visible_content;

  _visible_content = ValueConverter::to_string(_lisp_value, _location);

  return previous_content != _visible_content;
}
