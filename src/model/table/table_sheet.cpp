#include "table_sheet.h"
#include "table_cell.h"
#include "table_column_definition.h"
#include "table_row_definition.h"
#include <cassert>
#include <cstddef>
#include <iostream>
#include <memory>
#include <stdexcept>

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

std::optional<TableCellPtr> TableSheet::get_cell(size_t row, size_t col) const {
  if (row < rows.size()) {
    auto &table_row = rows[row];

    if (table_row && col < table_row->size()) {
      return (*table_row)[col];
    }
  }

  return {};
}

std::pair<TableRowDefinitionPtr, TableColumnDefinitionPtr>
TableSheet::get_definitions_for_location(const Location &location) {
  return {};
}

bool TableSheet::move_cursor_left() {
  if (current_cell.x() == 0) {
    return false;
  }

  current_cell.moveLeft();

  return true;
}

bool TableSheet::move_cursor_right() {
  // TODO
  // if (current_cell.x() == MAX) {
  //   return;
  // }

  current_cell.moveRight();

  return true;
}

bool TableSheet::move_cursor_down() {
  // TODO
  // if (current_cell.y() == MAX) {
  //   return;
  // }

  current_cell.moveDown();

  return true;
}

bool TableSheet::move_cursor_up() {
  if (current_cell.y() == 0) {
    return false;
  }

  current_cell.moveUp();

  return true;
}

size_t TableSheet::num_rows() const {
  return row_definitions.size();
}

size_t TableSheet::num_cols() const  {
  return column_definitions.size();
}

TableCellPtr TableSheet::get_current_cell() const {
  auto cell = get_cell_by_location(current_cell);

  //assert(cell);
  if (!cell) {
    throw std::runtime_error("Fatal: Current cell is invalid.");
  }

  return *cell;
}

std::optional<TableCellPtr> TableSheet::get_cell_by_location(const Location& location) const {
  return get_cell(location.y(), location.x());
}
