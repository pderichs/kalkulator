#ifndef TABLE_SHEET_INCLUDED
#define TABLE_SHEET_INCLUDED

#include <memory>
#include <string>
#include <vector>

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
};

typedef std::shared_ptr<TableSheet> TableSheetPtr;
typedef std::vector<TableSheetPtr> TableSheets;

#endif
