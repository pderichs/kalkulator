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

#include "table_sheet.h"
#include "table_cell.h"
#include "table_change_history.h"
#include "table_column_definition.h"
#include "table_row_definition.h"
#include <iostream>
#include <memory>
#include <queue>
#include <sstream>
#include <stdexcept>

const size_t INITIAL_ROW_COUNT = 100;
const size_t INITIAL_COL_COUNT = 100;
const size_t ROW_PAGE_MOVE_AMOUNT = 10;

TableSheet::TableSheet(const std::string &param_name)
    : _column_definitions(), _row_definitions(), _rows(), _name(param_name),
      _current_cell(0, 0), _change_history(), _row_top_positions(),
      _col_left_positions() {
  for (size_t c = 0; c < INITIAL_COL_COUNT; c++) {
    _column_definitions.push_back(std::make_shared<TableColumnDefinition>());
  }

  for (size_t c = 0; c < INITIAL_ROW_COUNT; c++) {
    _row_definitions.push_back(std::make_shared<TableRowDefinition>());
  }

  // Create initial rows and cols
  for (size_t r = 0; r < INITIAL_ROW_COUNT; r++) {
    auto row = std::make_shared<TableRow>();

    for (size_t c = 0; c < INITIAL_COL_COUNT; c++) {
      row->push_back(std::make_shared<TableCell>(r, c));
    }

    _rows.push_back(row);
  }
}

size_t TableSheet::row_count() const { return INITIAL_ROW_COUNT; }

size_t TableSheet::col_count() const { return INITIAL_COL_COUNT; }

TableCellPtr TableSheet::get_cell(size_t row, size_t col) const {
  if (row < _rows.size()) {
    auto &table_row = _rows[row];

    if (table_row && col < table_row->size()) {
      return (*table_row)[col];
    }
  }

  return {};
}

std::pair<TableRowDefinitionPtr, TableColumnDefinitionPtr>
TableSheet::get_definitions_for_location(const Location &location) const {
  return {_row_definitions[location.y()], _column_definitions[location.x()]};
}

bool TableSheet::move_cursor_left() {
  if (_current_cell.x() == 0) {
    return false;
  }

  _current_cell.moveLeft();

  return true;
}

bool TableSheet::move_cursor_right() {
  if ((size_t)_current_cell.x() >= get_max_col()) {
    return false;
  }

  _current_cell.moveRight();

  return true;
}

size_t TableSheet::get_max_row() const { return _row_definitions.size() - 1; }

bool TableSheet::move_cursor_down() {
  if ((size_t)_current_cell.y() >= get_max_row()) {
    return false;
  }

  _current_cell.moveDown();

  return true;
}

bool TableSheet::move_cursor_up() {
  if (_current_cell.y() == 0) {
    return false;
  }

  _current_cell.moveUp();

  return true;
}

size_t TableSheet::num_rows() const { return _row_definitions.size(); }

size_t TableSheet::num_cols() const { return _column_definitions.size(); }

TableCellPtr TableSheet::get_current_cell() const {
  auto cell = get_cell_by_location(_current_cell);

  // assert(cell);
  if (!cell) {
    std::stringstream ss;
    ss << "Fatal: Current cell is invalid: ";
    ss << _current_cell.x() << ", " << _current_cell.y();
    throw std::runtime_error(ss.str());
  }

  return cell;
}

TableCellPtr TableSheet::get_cell_by_location(const Location &location) const {
  return get_cell(location.y(), location.x());
}

bool TableSheet::move_cursor_page_up() {
  if (_current_cell.y() == 0) {
    return false;
  }

  int amount = ROW_PAGE_MOVE_AMOUNT;
  if (_current_cell.y() - amount < 0) {
    amount = _current_cell.y();
  }

  _current_cell.moveUp(amount);

  return true;
}

bool TableSheet::move_cursor_page_down() {
  if (static_cast<size_t>(_current_cell.y()) == get_max_row()) {
    return false;
  }

  size_t amount = ROW_PAGE_MOVE_AMOUNT;
  if (_current_cell.y() + amount > get_max_row()) {
    // Move to last row
    _current_cell = Location(_current_cell.x(), get_max_row());
    return true;
  }

  _current_cell.moveDown(amount);
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

  _current_cell = cell;

  return true;
}

void TableSheet::clear_current_cell() {
  auto cell = get_current_cell();
  std::string previous_content = cell->get_formula_content();
  cell->clear();

  CellState state{cell->location(), previous_content, ""};
  StateHistoryItemPtr item = std::make_shared<StateHistoryItem>(state);
  _change_history.push_state(item);
}

bool TableSheet::update_content(const Location &cell_location,
                                const std::string &content,
                                UpdateIdType update_id) {
  auto cell = get_cell_by_location(cell_location);

  std::string previous_content = cell->get_formula_content();

  // Note: update listeners are applied within the call to update_content
  if (cell->update_content(content, _name, update_id)) {
    CellState state{cell_location, previous_content, content};
    StateHistoryItemPtr item = std::make_shared<StateHistoryItem>(state);
    _change_history.push_state(item);

    return true;
  }

  return false;
}

size_t TableSheet::get_max_col() const {
  return _column_definitions.size() - 1;
}

StateHistoryItemPtr TableSheet::undo() {
  return _change_history.undo();
}

StateHistoryItemPtr TableSheet::redo() {
  return _change_history.redo();
}

size_t TableSheet::get_current_column_width() const {
  TableColumnDefinitionPtr col_def = _column_definitions[_current_cell.x()];
  return col_def->width;
}

size_t TableSheet::get_current_row_height() const {
  TableRowDefinitionPtr row_def = _row_definitions[_current_cell.y()];
  return row_def->height;
}

void TableSheet::set_column_width(size_t idx, size_t width) {
  // Clear offsets cache
  _col_left_positions.clear();

  TableColumnDefinitionPtr col_def = _column_definitions[idx];
  col_def->width = width;
}

void TableSheet::set_row_height(size_t idx, size_t height) {
  // Clear offsets cache
  _row_top_positions.clear();

  TableRowDefinitionPtr row_def = _row_definitions[idx];
  row_def->height = height;
}

void TableSheet::set_current_column_width(size_t width) {
  set_column_width(_current_cell.x(), width);
}

void TableSheet::set_current_row_height(size_t height) {
  set_row_height(_current_cell.y(), height);
}

void TableSheet::set_current_cell_format(const TableCellFormat &format) const {
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

LocationSet TableSheet::search(const std::string &search_term) const {
  LocationSet result;

  for (size_t row = 0; row < num_rows(); row++) {
    for (size_t col = 0; col < num_cols(); col++) {
      const auto &cell = get_cell(row, col);
      if (cell->has_content()) {
        std::string content = cell->visible_content();
        if (content.find(search_term) != content.npos) {
          result.insert(cell->location());
        }
      }
    }
  }

  return result;
}

size_t TableSheet::get_row_top_position(size_t row) {
  auto it = _row_top_positions.find(row);
  if (it != _row_top_positions.end()) {
    return it->second;
  }

  size_t r = 0;
  size_t top = 0;
  for (const auto &rowdef : _row_definitions) {
    if (r == row) {
      break;
    }

    top += rowdef->height;
    r++;
  }

  _row_top_positions[row] = top;

  return top;
}

size_t TableSheet::get_col_left_position(size_t col) {
  auto it = _col_left_positions.find(col);
  if (it != _col_left_positions.end()) {
    return it->second;
  }

  size_t c = 0;
  size_t left = 0;
  for (const auto &coldef : _column_definitions) {
    if (c == col) {
      break;
    }

    left += coldef->width;
    c++;
  }

  _col_left_positions[col] = left;

  return left;
}
