#ifndef TABLE_SHEET_INCLUDED
#define TABLE_SHEET_INCLUDED

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "location.h"
#include "table_cell.h"
#include "table_change_history.h"
#include "table_column_definition.h"
#include "table_row_definition.h"
#include "table_selections.h"

typedef std::vector<TableRowPtr> TableRows;

struct TableSheet {
  TableSheet(const std::string &name);
  ~TableSheet();

  TableColumnDefinitions column_definitions;
  TableRowDefinitions row_definitions;
  TableRows rows;
  std::string name;
  TableSelections selections;
  Location current_cell;
  TableSheetChangeHistory change_history;

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

  void update_content(const Location& cell_location, const std::string& content);
};

typedef std::shared_ptr<TableSheet> TableSheetPtr;
typedef std::vector<TableSheetPtr> TableSheets;

#endif
