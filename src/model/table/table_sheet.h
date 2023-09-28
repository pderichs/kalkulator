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
#include <cstddef>
#include <map>
#include <memory>
#include <string>
#include <vector>

typedef std::vector<TableRowPtr> TableRows;

typedef std::map<Location, LocationSet> ListenersMap;

struct TableSheet {
  TableSheet(const std::string &name);

  TableColumnDefinitions column_definitions;
  TableRowDefinitions row_definitions;
  TableRows rows;
  std::string name;
  TableSelections selections;
  Location current_cell;
  TableSheetChangeHistory change_history;
  ListenersMap _listener_map;

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

  void update_content(const Location &cell_location,
                      const std::string &content);

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

  void add_update_listener(const Location &listener,
                           const Location &listening_to);

  LocationSet search(const std::string &search_term) const;

private:
  void apply_state_change_item(const StateHistoryItemPtr &state) const;
  void trigger_listeners(const Location &Location);
  void remove_from_update_listeners(const Location &location);
};

typedef std::shared_ptr<TableSheet> TableSheetPtr;
typedef std::vector<TableSheetPtr> TableSheets;

#endif
