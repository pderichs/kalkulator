#include "table_sheet.h"
#include "table_cell.h"
#include "table_column_definition.h"
#include "table_row_definition.h"
#include <cstddef>
#include <iostream>
#include <memory>

const size_t INITIAL_ROW_COUNT = 100;
const size_t INITIAL_COL_COUNT = 100;

TableSheet::TableSheet(const std::string &name): current_cell(0, 0) {
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

void TableSheet::move_cursor_left() {
  if (current_cell.x() == 0) {
    return;
  }

  current_cell.moveLeft();
}

void TableSheet::move_cursor_right() {
  // TODO
  // if (current_cell.x() == MAX) {
  //   return;
  // }

  current_cell.moveRight();
}

void TableSheet::move_cursor_down() {
  // TODO
  // if (current_cell.y() == MAX) {
  //   return;
  // }

  current_cell.moveDown();
}

void TableSheet::move_cursor_up() {
  if (current_cell.y() == 0) {
    return;
  }

  current_cell.moveUp();
}

size_t TableSheet::num_rows() const {
  return row_definitions.size();
}

size_t TableSheet::num_cols() const  {
  return column_definitions.size();
}
