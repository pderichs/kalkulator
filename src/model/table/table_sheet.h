#ifndef TABLE_SHEET_INCLUDED
#define TABLE_SHEET_INCLUDED

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "location.h"
#include "table_cell.h"
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

  size_t row_count() const;
  size_t col_count() const;
  std::optional<TableCellPtr> get_cell(size_t row, size_t col);
  std::pair<TableRowDefinitionPtr, TableColumnDefinitionPtr>
  get_definitions_for_location(const Location &location);

  void move_cursor_left();
  void move_cursor_right();
  void move_cursor_down();
  void move_cursor_up();
};

typedef std::shared_ptr<TableSheet> TableSheetPtr;
typedef std::vector<TableSheetPtr> TableSheets;

#endif
