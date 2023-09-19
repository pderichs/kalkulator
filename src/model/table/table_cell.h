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

#ifndef TABLE_CELL_INCLUDED
#define TABLE_CELL_INCLUDED

#include <memory>
#include <string>
#include <vector>

#include "../lisp/value_converter.h"

#include "location.h"
#include "table_cell_format.h"

class TableCell {
public:
  TableCell(int row, int col)
      : _formula_content(), _visible_content(), _lisp_value(),
        _location(col, row), _format() {}

  bool update_content(const std::string &content);
  [[nodiscard]] std::string visible_content() const;
  [[nodiscard]] std::string get_formula_content() const;
  bool is_formula() const;
  [[nodiscard]] LispValuePtr lisp_value() const { return _lisp_value; }
  [[nodiscard]] bool has_content() const;
  void clear();
  [[nodiscard]] long row() const { return _location.y(); }
  [[nodiscard]] long col() const { return _location.x(); }
  [[nodiscard]] Location location() const { return _location; }

  [[nodiscard]] bool has_format() const { return static_cast<bool>(_format); }
  [[nodiscard]] TableCellFormat format() const { return *_format; }
  void set_format(const TableCellFormat &format) { _format = format; }

  bool recalc();

private:
  std::string _formula_content;
  std::string _visible_content;

  LispValuePtr _lisp_value;

  // Cell knows about its position
  const Location _location;

  std::optional<TableCellFormat> _format;
};

typedef std::shared_ptr<TableCell> TableCellPtr;
typedef std::vector<TableCellPtr> TableRow;
typedef std::shared_ptr<TableRow> TableRowPtr;

#endif
