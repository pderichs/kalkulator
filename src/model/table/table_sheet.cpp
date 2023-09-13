#include "table_sheet.h"
#include "table_cell.h"
#include "table_change_history.h"
#include "table_column_definition.h"
#include "table_row_definition.h"
#include <cassert>
#include <cstddef>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>

const size_t INITIAL_ROW_COUNT = 100;
const size_t INITIAL_COL_COUNT = 100;
const size_t ROW_PAGE_MOVE_AMOUNT = 10;

TableSheet::TableSheet(const std::string &name) : current_cell(0, 0) {
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
      row->push_back(std::make_shared<TableCell>(r, c));
    }

    rows.push_back(row);
  }
}

size_t TableSheet::row_count() const { return INITIAL_ROW_COUNT; }

size_t TableSheet::col_count() const { return INITIAL_COL_COUNT; }

TableCellPtr TableSheet::get_cell(size_t row, size_t col) const {
  if (row < rows.size()) {
    auto &table_row = rows[row];

    if (table_row && col < table_row->size()) {
      return (*table_row)[col];
    }
  }

  return {};
}

std::pair<TableRowDefinitionPtr, TableColumnDefinitionPtr>
TableSheet::get_definitions_for_location(const Location &location) const {
  return {row_definitions[location.y()], column_definitions[location.x()]};
}

bool TableSheet::move_cursor_left() {
  if (current_cell.x() == 0) {
    return false;
  }

  current_cell.moveLeft();

  return true;
}

bool TableSheet::move_cursor_right() {
  if ((size_t)current_cell.x() >= get_max_col()) {
    return false;
  }

  current_cell.moveRight();

  return true;
}

size_t TableSheet::get_max_row() const { return row_definitions.size() - 1; }

bool TableSheet::move_cursor_down() {
  if ((size_t)current_cell.y() >= get_max_row()) {
    return false;
  }

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

size_t TableSheet::num_rows() const { return row_definitions.size(); }

size_t TableSheet::num_cols() const { return column_definitions.size(); }

TableCellPtr TableSheet::get_current_cell() const {
  auto cell = get_cell_by_location(current_cell);

  // assert(cell);
  if (!cell) {
    std::stringstream ss;
    ss << "Fatal: Current cell is invalid: ";
    ss << current_cell.x() << ", " << current_cell.y();
    throw std::runtime_error(ss.str());
  }

  return cell;
}

TableCellPtr TableSheet::get_cell_by_location(const Location &location) const {
  return get_cell(location.y(), location.x());
}

bool TableSheet::move_cursor_page_up() {
  if (current_cell.y() == 0) {
    return false;
  }

  int amount = ROW_PAGE_MOVE_AMOUNT;
  if (current_cell.y() - amount < 0) {
    amount = current_cell.y();
  }

  current_cell.moveUp(amount);

  return true;
}

bool TableSheet::move_cursor_page_down() {
  size_t amount = ROW_PAGE_MOVE_AMOUNT;
  if (current_cell.y() + amount > get_max_row()) {
    // Move to last row
    current_cell = Location(current_cell.x(), get_max_row());
    return true;
  }

  current_cell.moveDown(amount);
  return true;
}

bool TableSheet::is_in_bounds(const Location &cell) const {
  return cell.x() >= 0 && (size_t)cell.x() < num_cols() && cell.y() >= 0 &&
         (size_t)cell.y() < num_rows();
}

bool TableSheet::select_cell(const Location &cell) {
  if (!is_in_bounds(cell)) {
    return false;
  }

  current_cell = cell;

  return true;
}

void TableSheet::clear_current_cell() {
  auto cell = get_current_cell();
  std::string previous_content = cell->get_formula_content();
  cell->clear();

  CellState state{cell->location(), previous_content, ""};
  StateHistoryItemPtr item = std::make_shared<StateHistoryItem>(state);
  change_history.push_state(item);
}

void TableSheet::update_content(const Location &cell_location,
                                const std::string &content) {
  auto cell = get_cell_by_location(cell_location);

  std::string previous_content = cell->get_formula_content();
  cell->update_content(content);

  CellState state{cell_location, previous_content, content};
  StateHistoryItemPtr item = std::make_shared<StateHistoryItem>(state);
  change_history.push_state(item);
}

size_t TableSheet::get_max_col() const { return column_definitions.size() - 1; }

void TableSheet::apply_state_change_item(const StateHistoryItemPtr &state) {
  for (const auto &cell_state : state->cell_states) {
    auto cell = get_cell_by_location(cell_state.location);
    cell->update_content(cell_state.prev);
  }
}

void TableSheet::undo() {
  StateHistoryItemPtr state = change_history.undo();
  if (!state) {
    return;
  }
  apply_state_change_item(state);
}

void TableSheet::redo() {
  StateHistoryItemPtr state = change_history.redo();
  if (!state) {
    return;
  }
  apply_state_change_item(state);
}

size_t TableSheet::get_current_column_width() const {
  TableColumnDefinitionPtr col_def = column_definitions[current_cell.x()];
  return col_def->width;
}

size_t TableSheet::get_current_row_height() const {
  TableRowDefinitionPtr row_def = row_definitions[current_cell.y()];
  return row_def->height;
}

void TableSheet::set_column_width(size_t idx, size_t width) {
  TableColumnDefinitionPtr col_def = column_definitions[idx];
  col_def->width = width;
}

void TableSheet::set_row_height(size_t idx, size_t height) {
  TableRowDefinitionPtr row_def = row_definitions[idx];
  row_def->height = height;
}

void TableSheet::set_current_column_width(size_t width) {
  set_column_width(current_cell.x(), width);
}

void TableSheet::set_current_row_height(size_t height) {
  set_row_height(current_cell.y(), height);
}

void TableSheet::set_current_cell_format(const TableCellFormat &format) {
  TableCellPtr cell = get_current_cell();
  cell->set_format(format);
}

std::optional<TableCellFormat> TableSheet::get_current_cell_format() const {
  TableCellPtr cell = get_current_cell();

  if (cell->has_format()) {
    return cell->format();
  }

  return {};
}
