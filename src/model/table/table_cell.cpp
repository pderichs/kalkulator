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
#include "table_cell_location.h"

bool TableCell::update_content(const std::string &content,
                               const std::string &sheet_name,
                               UpdateIdType update_id) {
  _lisp_value = ValueConverter::to_lisp_value(content);

  if (_lisp_value->is_function()) {
    _formula_content = content;
  } else {
    _formula_content.clear();
  }

  return recalc(sheet_name, update_id);
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

void TableCell::clear() {
  _lisp_value = {};
  _cached_result = {};
  _last_update = 0;
}

// Returns true if content changed, false otherwise.
bool TableCell::recalc(const std::string &sheet_name, UpdateIdType update_id) {
  if (_last_update == update_id) {
    return false;
  }

  std::string previous_content = _visible_content;

  auto location = TableCellLocation(sheet_name,
                                    _location);

  if (is_formula()) {
    _cached_result =
        ValueConverter::execute_function(_lisp_value, location, update_id);
  }

  _visible_content = ValueConverter::to_string(result(),
                                               location,
                                               update_id);

  _last_update = update_id;

  return previous_content != _visible_content;
}

LispValuePtr TableCell::result() const {
  if (is_formula() && _cached_result) {
    return _cached_result;
  }

  return _lisp_value;
}
