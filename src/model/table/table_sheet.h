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
#include "model/lisp/update_id.h"
#include "table_cell_selection.h"
#include <cstddef>
#include <map>
#include <memory>
#include <string>
#include <vector>

typedef std::vector<TableRowPtr> TableRows;

/**
 * Represents one sheet within the spread sheet application model.
 */
class TableSheet {
public:
  TableSheet(const std::string &name);

  /**
   * Request a pointer to the cell object at row and col.
   *
   * @param row Row coordinate of the cell within the sheet
   * @param col Column coordinate of the cell within the sheet
   * @return A pointer to the cell on location row / col
   */
  TableCellPtr get_cell(size_t row, size_t col) const;

  /**
   * Request a pointer to the cell object at given location.
   *
   * @param location Location of the cell
   * @return A pointer to the cell on location
   */
  TableCellPtr get_cell(const Location &location) const;

  /**
   * @return A pointer to the current active cell
   */
  TableCellPtr get_current_cell() const;

  /**
   * Retrieves the definitions for the current location
   * @param location Location to query definitions for
   * @return A pair of row and column definition data
   */
  std::pair<TableRowDefinitionPtr, TableColumnDefinitionPtr>
  get_definitions_for_location(const Location &location) const;

  /**
   * @return the current row count
   */
  size_t row_count() const;

  /**
   * @return the current column count
   */
  size_t col_count() const;

  /**
   * Moves the cursor left
   *
   * @return true if cursor was moved, false otherwise
   */
  bool move_cursor_left();

  /**
   * Moves the cursor right
   *
   * @return true if cursor was moved, false otherwise
   */
  bool move_cursor_right();

  /**
   * Moves the cursor down
   *
   * @return true if cursor was moved, false otherwise
   */
  bool move_cursor_down();

  /**
   * Moves the cursor up
   *
   * @return true if cursor was moved, false otherwise
   */
  bool move_cursor_up();

  /**
   * Moves the cursor one page up
   *
   * @return true if cursor was moved, false otherwise
   */
  bool move_cursor_page_up();

  /**
   * Moves the cursor one page down
   *
   * @return true if cursor was moved, false otherwise
   */
  bool move_cursor_page_down();

  /**
   * Selects the cell on the given location
   *
   * @param cell Location to select
   * @return true if cell was selected, false otherwise
   */
  bool select_cell(const Location &cell);

  /**
   * Checks whether the given location is in bounds within the sheet
   *
   * @param cell Location to be checked
   * @return true if location is in bounds, false otherwise
   */
  bool is_in_bounds(const Location &cell) const;

  /**
   * Clears the current cell
   */
  LocationSet clear_current_cells();

  /**
   * @return the maximum row coordinate
   */
  size_t get_max_row() const;

  /**
   * @return the maximum column coordinate
   */
  size_t get_max_col() const;

  /**
   * Updates the content of the cell
   *
   * @param cell_location Cell to be updated
   * @param content Content to be assigned
   * @param update_id Id of the current operation
   * @return true if content was updated, false otherwise
   */
  bool update_content(const Location &cell_location,
                      const std::string &content, UpdateIdType update_id);

  /**
   * @return the next state for an undo operation on this sheet
   */
  StateHistoryItemPtr undo();

  /**
   * @return the next state for a redo operation on this sheet
   */
  StateHistoryItemPtr redo();

  /**
   * @return the current column width
   */
  size_t get_current_column_width() const;

  /**
   * @return the current row height
   */
  size_t get_current_row_height() const;

  /**
   * Updates the column width
   *
   * @param idx Index of the column to update
   * @param width New width to assign
   */
  void set_column_width(size_t idx, size_t width);

  /**
   * Updates the row height
   *
   * @param idx Index of the row to update
   * @param height New height to assign
   */
  void set_row_height(size_t idx, size_t height);

  /**
   * Updates the current column width
   *
   * @param width New width to assign
   */
  void set_current_column_width(size_t width);

  /**
   * Updates the current row height
   * @param height New height to assign
   */
  void set_current_row_height(size_t height);

  /**
   * Updates the current cell format
   *
   * @param format Format to assign
   */
  void set_cell_format(const TableCellFormat &format) const;

  /**
   * Updates the current cell comment
   *
   * @param comment Comment to assign
   */
  void set_cell_comment(const std::string &comment) const;

  /**
   * @return the format of the current cell
   */
  std::optional<TableCellFormat> get_current_cell_format() const;

  /**
   * Searches this sheet for a given string
   *
   * @param search_term Term to search for
   * @return A set of locations matching the search
   */
  LocationSet search(const std::string &search_term) const;

  /**
   * @return location of the current active cell
   */
  TableCellSelection selection() const { return _selected_cells; }

  /**
   * @return the name of the current sheet
   */
  std::string name() const { return _name; }

  /**
   * @return the row definitions
   */
  TableRowDefinitions row_definitions() const { return _row_definitions; }

  /**
   * @return the column definitions
   */
  TableColumnDefinitions column_definitions() const { return _column_definitions; }

  /**
   * Returns the top pixel position of the requested row.
   *
   * @param row Row to get the top pixel position for
   * @return Top pixel position for row
   */
  size_t get_row_top_position(size_t row);

  /**
   * Returns the left pixel position of the requested column.
   *
   * @param col Column to get the left pixel position for
   * @return Left pixel position for column
   */
  size_t get_col_left_position(size_t col);

  /**
   * Adds a cell to the current selection of the sheet.
   *
   * @param location Location to be added to the selection
   */
  void selection_toggle_additional_cell(const Location &location);

private:
  TableColumnDefinitions _column_definitions;
  TableRowDefinitions _row_definitions;
  TableRows _rows;
  std::string _name;
  TableCellSelection _selected_cells;
  TableSheetChangeHistory _change_history;
  std::map<size_t, size_t> _row_top_positions;
  std::map<size_t, size_t> _col_left_positions;
};

typedef std::shared_ptr<TableSheet> TableSheetPtr;
typedef std::vector<TableSheetPtr> TableSheets;

#endif
