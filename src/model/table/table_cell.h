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
#include "table_cell_comment.h"
#include "model/lisp/update_id.h"

class TableCell {
public:
  TableCell(int row, int col)
      : _formula_content(), _visible_content(), _lisp_value(), _cached_result(),
        _last_update(0), _location(col, row), _format(), _comment() {}

  bool update_content(const std::string &content,
                      const std::string &sheet_name,
                      UpdateIdType update_id);
  std::string visible_content() const;
  std::string get_formula_content() const;
  bool is_formula() const;
  LispValuePtr lisp_value() const { return _lisp_value; }
  LispValuePtr result() const;

  bool has_content() const;
  void clear();
  long row() const { return _location.y(); }
  long col() const { return _location.x(); }
  Location location() const { return _location; }

  bool has_format() const { return static_cast<bool>(_format); }
  TableCellFormat format() const { return *_format; }
  void set_format(const TableCellFormat &format) { _format = format; }

  bool has_comment() const { return static_cast<bool>(_comment); }
  TableCellComment comment() const { return *_comment; }
  void set_comment(const TableCellComment &comment) { _comment = comment; }

  bool recalc(const std::string &sheet_name, UpdateIdType update_id);

private:
  std::string _formula_content;
  std::string _visible_content;

  LispValuePtr _lisp_value;

  LispValuePtr _cached_result;
  UpdateIdType _last_update;

  // Cell knows about its position
  const Location _location;

  std::optional<TableCellFormat> _format;
  std::optional<TableCellComment> _comment;
};

typedef std::shared_ptr<TableCell> TableCellPtr;
typedef std::vector<TableCellPtr> TableRow;
typedef std::shared_ptr<TableRow> TableRowPtr;

#endif
