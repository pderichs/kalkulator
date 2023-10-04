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

#ifndef TABLE_SHEET_INCLUDED
#define TABLE_SHEET_INCLUDED

#include "location.h"
#include "table_cell.h"
#include "table_cell_format.h"
#include "table_change_history.h"
#include "table_column_definition.h"
#include "table_row_definition.h"
#include "table_selections.h"
#include "model/lisp/update_id.h"
#include <cstddef>
#include <map>
#include <memory>
#include <string>
#include <vector>

typedef std::vector<TableRowPtr> TableRows;

class TableSheet {
public:
  TableSheet(const std::string &name);

  size_t row_count() const;
  size_t col_count() const;
  TableCellPtr get_cell(size_t row, size_t col) const;
  TableCellPtr get_cell_by_location(const Location &location) const;
  TableCellPtr get_current_cell() const;
  std::pair<TableRowDefinitionPtr, TableColumnDefinitionPtr>
  get_definitions_for_location(const Location &location) const;

  size_t num_rows() const;
  size_t num_cols() const;

  bool move_cursor_left();
  bool move_cursor_right();
  bool move_cursor_down();
  bool move_cursor_up();
  bool move_cursor_page_up();
  bool move_cursor_page_down();

  bool select_cell(const Location &cell);

  bool is_in_bounds(const Location &cell) const;

  void clear_current_cell();

  size_t get_max_row() const;
  size_t get_max_col() const;

  bool update_content(const Location &cell_location,
                      const std::string &content, UpdateIdType update_id);

  void undo();
  void redo();

  size_t get_current_column_width() const;
  size_t get_current_row_height() const;

  void set_column_width(size_t idx, size_t width);
  void set_row_height(size_t idx, size_t height);
  void set_current_column_width(size_t width);
  void set_current_row_height(size_t height);

  void set_current_cell_format(const TableCellFormat &format) const;
  std::optional<TableCellFormat> get_current_cell_format() const;

  LocationSet search(const std::string &search_term) const;

  Location current_cell() const { return _current_cell; }
  std::string name() const { return _name; }
  TableRowDefinitions row_definitions() const { return _row_definitions; }
  TableColumnDefinitions column_definitions() const { return _column_definitions; }

private:
  void apply_state_change_item(const StateHistoryItemPtr &state) const;
  void remove_from_update_listeners(const Location &location);

private:
  TableColumnDefinitions _column_definitions;
  TableRowDefinitions _row_definitions;
  TableRows _rows;
  std::string _name;
  TableSelections _selections;
  Location _current_cell;
  TableSheetChangeHistory _change_history;
};

typedef std::shared_ptr<TableSheet> TableSheetPtr;
typedef std::vector<TableSheetPtr> TableSheets;

#endif
