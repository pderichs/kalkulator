#include "table_sheet.h"
#include "table_cell.h"
#include "table_column_definition.h"
#include "table_row_definition.h"
#include <cstddef>
#include <iostream>
#include <memory>

const size_t INITIAL_ROW_COUNT = 100;
const size_t INITIAL_COL_COUNT = 100;

TableSheet::TableSheet(const std::string &name) {
  std::cout << "Constructor table sheet" << std::endl;

  this->name = name;

  for (size_t c = 0; c < INITIAL_COL_COUNT; c++) {
    column_definitions.push_back(std::make_shared<TableColumnDefinition>());
  }

  for (size_t c = 0; c < INITIAL_ROW_COUNT; c++) {
    row_definitions.push_back(std::make_shared<TableRowDefinition>());
  }

  // Create initial rows and cols
  for (size_t r = 0; r < INITIAL_ROW_COUNT; r++) {
    auto row = std::make_shared<TableRow>();

    for (size_t c = 0; c < INITIAL_COL_COUNT; c++) {
      row->push_back(std::make_shared<TableCell>());
    }

    rows.push_back(row);
  }
}

TableSheet::~TableSheet() {}

size_t TableSheet::row_count() const { return INITIAL_ROW_COUNT; }

size_t TableSheet::col_count() const { return INITIAL_COL_COUNT; }

std::optional<TableCellPtr> TableSheet::get_cell(size_t row, size_t col) {
  if (rows.size() < row) {
    TableRowPtr &table_row = rows[row];

    if (table_row && table_row->size() < col) {
      return (*table_row)[col];
    }
  }

  return {};
}

std::pair<TableRowDefinitionPtr, TableColumnDefinitionPtr>
TableSheet::get_definitions_for_location(const Location &location) {
  return {};
}
