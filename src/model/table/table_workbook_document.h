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

#ifndef WORKBOOK_DOCUMENT_INCLUDED
#define WORKBOOK_DOCUMENT_INCLUDED

#include "../event_sink.h"
#include "table_cell_location.h"
#include "table_cell.h"
#include "table_cell_format.h"
#include "table_search_result.h"
#include "table_sheet.h"
#include <memory>
#include <string>
#include <vector>

typedef std::vector<TableCellPtr> TableCellPtrVector;
typedef std::map<TableCellLocation, TableCellLocationSet> TableCellListenerMap;

/**
 * Represents a workbook within the spread sheet application model.
 *
 * It contains all sheets and functions to access the current sheet and cell
 * states.
 */
class TableWorkbookDocument {
public:
  explicit TableWorkbookDocument(EventSink *event_sink);
  ~TableWorkbookDocument() = default;

  // Delete copy constructor and assignment operator
  TableWorkbookDocument(const TableWorkbookDocument &other) = delete;
  TableWorkbookDocument &operator=(const TableWorkbookDocument &other) = delete;

  /**
   * Indicates whether this document has been changed
   *
   * @return true if changed, false otherwise
   */
  bool changed() const { return _changed; }

  /**
   * Returns the table sheet for the given name.
   *
   * @param name Name of the sheet
   * @return Pointer to the sheet
   */
  TableSheetPtr table_sheet_by_name(const std::string &name) const;

  /**
   * @return Pointer to the currently active sheet
   */
  TableSheetPtr current_sheet() const { return _current_sheet; }

  /**
   * Initializes the workbook
   */
  void initialize();

  /**
   * Helper function: combines calls to clear and initialize.
   */
  void clear_and_initialize();

  /**
   * Clears the workbook data of this instance.
   */
  void clear();

  /**
   * Adds a new sheet to the workbook.
   *
   * @param name Name of the new sheet
   * @return Pointer to the newly created sheet
   */
  TableSheetPtr add_sheet(const std::string &name);

  /**
   * Removes the currently active sheet and sets another sheet as the active
   * one.
   */
  void remove_current_sheet();

  /**
   * Resets the changed flag of this workbook.
   */
  void clear_changed_flag() { _changed = false; }

  /**
   * Updates the active sheet
   * @param name Name of the sheet to activate
   */
  void set_active_sheet(const std::string &name);

  /**
   * Sets the active cell for the given sheet
   * @param sheet_name Name of the sheet on which the cell should be activated
   * @param current_cell Cell location identifying the new active cell
   */
  void set_current_cell(const std::string &sheet_name,
                        const Location &current_cell);

  /**
   * Updates the content of the given cell and handles the needed updates
   * within the model.
   *
   * @param sheet Sheet of the cell to be updated
   * @param cell_location Location of the cell to be updated
   * @param content New content to be assigned for the cell
   * @param update_id Identifies the current update operation
   */
  void update_cell_content(const TableSheetPtr &sheet,
                           Location cell_location,
                           const std::string &content,
                           unsigned long update_id);

  /**
   * Clears the current cell and handles the needed updates to the model.
   */
  void clear_current_cell();

  /**
   * Moves the cursor up.
   *
   * @return true if cursor was able to move, false otherwise.
   */
  bool move_cursor_up();

  /**
   * Moves the cursor down.
   *
   * @return true if cursor was able to move, false otherwise.
   */
  bool move_cursor_down();

  /**
   * Moves the cursor left.
   *
   * @return true if cursor was able to move, false otherwise.
   */
  bool move_cursor_left();

  /**
   * Moves the cursor right.
   *
   * @return true if cursor was able to move, false otherwise.
   */
  bool move_cursor_right();

  /**
   * Moves the cursor one page up.
   *
   * @return true if cursor was able to move, false otherwise.
   */
  bool move_cursor_page_up();

  /**
   * Moves the cursor one page down.
   *
   * @return true if cursor was able to move, false otherwise.
   */
  bool move_cursor_page_down();

  /**
   * Searches the sheets in the workbook for the given string.
   *
   * @param search_term String to search for
   * @return Result of the search
   */
  TableSearchResult search_sheets(const std::string &search_term) const;

  /**
   * @return The current selected cell location within the current sheet
   */
  Location current_sheet_selected_cell() const;

  /**
   * Returns the cell located at the provided pixel position.
   *
   * @param position Pixel position to be queried
   * @return The cell location (col, row) within the sheet
   */
  Location get_cell_by_pos(const Location &position) const;

  /**
   * Selects the given cell location within the current sheet
   *
   * @param cell Cell location to be selected
   */
  void select_cell(const Location &cell);

  /**
   * Selects the given cell location within the current sheet
   *
   * @param col Column of the cell to be selected
   * @param row Row of the cell to be selected
   */
  void select_cell(size_t row, size_t col);

  /**
   * Selects the sheet with the provided name
   *
   * @param sheet_name Name of the sheet to be selected
   * @return true if selection changed, false otherwise
   */
  bool select_sheet_by_name(const std::string &sheet_name);

  /**
   * Selects the provided sheet
   *
   * @param sheet Sheet to be selected
   * @return true if selection changed, false otherwise
   */
  bool select_sheet(const TableSheetPtr &sheet);

  /**
   * Finds the sheet identified by the provided name
   *
   * @param sheet_name Name of the sheet to find
   * @return A pointer to the sheet instance identified by the provided name
   */
  TableSheetPtr find_sheet_by_name(const std::string &sheet_name) const;

  /**
   * Provides a way to get the cell pointer identified by sheet and location.
   *
   * @param location Location of the cell
   * @param sheet_name Optional sheet name
   * @return A pointer to the cell
   */
  TableCellPtr get_cell(const Location &location,
                        const std::string &sheet_name = "") const;

  /**
   * @return a pointer to the current cell of the current active sheet
   */
  TableCellPtr get_current_cell() const;

  /**
   * Returns the cell pointers located within a range in the current sheet
   *
   * @param from Cell location range from
   * @param to Cell location range to
   * @return The cell pointers located within the provided range
   */
  TableCellPtrVector get_range(const Location &from, const Location &to) const;

  /**
   * Updates the current cell content within the currently active sheet
   * @param content Content to be assigned to the cell
   */
  void update_content_current_cell(const std::string &content);

  /**
   * @return the pixel width of the current sheet
   */
  int get_current_sheet_width() const;

  /**
   * @return the pixel height of the current sheet
   */
  int get_current_sheet_height() const;

  /**
   * @return the pixel width of the currently selected column in the current sheet
   */
  size_t get_current_column_width() const;

  /**
   * @return the pixel height of the currently selected row in the current sheet
   */
  size_t get_current_row_height() const;

  /**
   * Updates the current column size in the current sheet
   *
   * @param width Width to be assigned to the current column
   */
  void set_current_column_width(size_t width);

  /**
   * Updates the current row size in the current sheet
   *
   * @param height Height to be assigned to the current row
   */
  void set_current_row_height(size_t height);

  /**
   * @return A collection of all available sheets
   */
  TableSheets sheets() const { return _sheets; }

  /**
   * Updates the current file path of the workbook
   * @param path Path to be assigned
   */
  void set_file_path(const std::string &path) { _path = path; }

  /**
   * @return the current file path of the workbook
   */
  std::string file_path() const { return _path; }

  /**
   * Returns the row and column definitions of the current sheet's location
   *
   * @param location Cell location within the current sheet
   * @return The row and column definitions of the provided cell location
   */
  std::pair<TableRowDefinitionPtr, TableColumnDefinitionPtr>
  get_definitions_for_location(const Location &location) const {
    return _current_sheet->get_definitions_for_location(location);
  }

  /**
   * Undo of the last operation in the current sheet
   */
  void undo();

  /**
   * Redo of the last operation in the current sheet
   */
  void redo();

  /**
   * Updates the cell format of the current cell within the current sheet
   *
   * @param format Format to be assigned to the cell
   */
  void set_current_cell_format(const TableCellFormat &format);

  /**
   * @return An optional cell format of the current cell within the current sheet
   */
  std::optional<TableCellFormat> get_current_cell_format() const;

  /**
   * Adds an update listener to a cell
   *
   * @param listener Cell location of the listener cell
   * @param listening_to Cell location on which the listener is interested in
   */
  void add_update_listener(const TableCellLocation &listener,
                           const TableCellLocation &listening_to);

  /**
   * get_cell but with TableCellLocation.
   *
   * @param location Location of a cell within the workbook
   * @return A pointer to the cell
   */
  TableCellPtr get_cell_by_location(const TableCellLocation &location) const;

  /**
   * Returns the top pixel position of the requested row within the
   * current sheet.
   *
   * @param row Row to get the top pixel position for
   * @return Top pixel position for row
   */
  size_t get_row_top_position(size_t row);

  /**
   * Returns the left pixel position of the requested column within the
   * current sheet.
   *
   * @param col Column to get the left pixel position for
   * @return Left pixel position for column
   */
  size_t get_col_left_position(size_t col);

private:
  /**
   * Trigger all cell listeners which are registered for receiving updates on
   * the given cell location.
   *
   * @param location Location for which we want to trigger listeners
   * @param update_id Id which identifies the current operation
   */
  void trigger_listeners(const TableCellLocation &location,
                         UpdateIdType update_id);

  /**
   * Removes the cell location from listeners
   *
   * @param location Location of the listener to be removed
   */
  void remove_from_update_listeners(const TableCellLocation &location);

  /**
   * Helper function for undo/redo. Applies a change state history item.
   *
   * @param state_history_item Item which describes the change to apply
   * @param update_id Id which identifies the current operation
   */
  void apply_state_change_item(const StateHistoryItemPtr &state_history_item,
                               UpdateIdType update_id);

private:
  std::string _path;
  bool _changed;
  TableSheets _sheets;
  EventSink *_event_sink;
  TableSheetPtr _current_sheet;
  TableCellListenerMap _listeners;
};

typedef std::shared_ptr<TableWorkbookDocument> TableWorkbookDocumentPtr;

#endif
