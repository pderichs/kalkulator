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

#include "table_workbook_document.h"
#include "table_cell.h"
#include "table_sheet.h"
#include "model/lisp/value_converter.h"
#include "model/lisp/update_id.h"
#include <algorithm>
#include <memory>
#include <queue>

TableWorkbookDocument::TableWorkbookDocument(EventSink *event_sink)
    : _path(), _changed(false), _sheets(), _event_sink(event_sink),
      _current_sheet(), _listeners() {
  initialize();
}

TableSheetPtr
TableWorkbookDocument::table_sheet_by_name(const std::string &name) const {
  for (auto sheet : _sheets) {
    if (sheet->name() == name) {
      return sheet;
    }
  }

  return {};
}

void TableWorkbookDocument::remove_from_update_listeners(const TableCellLocation &location) {
  for (auto &it : _listeners) {
    it.second.erase(location);
  }
}

void TableWorkbookDocument::update_cell_content(const TableSheetPtr &sheet,
                                                Location cell_location,
                                                const std::string &content,
                                                unsigned long update_id) {
  auto location = TableCellLocation(sheet->name(), cell_location);
  remove_from_update_listeners(location);

  if (sheet->update_content(cell_location, content, update_id)) {
    trigger_listeners(location, update_id);

    _changed = true;

    std::any param = cell_location;
    _event_sink->send_event(CELL_UPDATED, param);
  }
}

void TableWorkbookDocument::update_content_current_cell(
    const std::string &content) {
  UpdateIdType update_id = generate_update_id();

  TableSheetPtr sheet = _current_sheet;

  update_cell_content(sheet, sheet->current_cell(), content, update_id);
}

bool TableWorkbookDocument::move_cursor_up() {
  TableSheetPtr sheet = _current_sheet;
  if (sheet->move_cursor_up()) {
    _event_sink->send_event(CURRENT_CELL_LOCATION_UPDATED,
                            sheet->current_cell());
    return true;
  }

  return false;
}

bool TableWorkbookDocument::move_cursor_down() {
  TableSheetPtr sheet = _current_sheet;
  if (sheet->move_cursor_down()) {
    _event_sink->send_event(CURRENT_CELL_LOCATION_UPDATED,
                            sheet->current_cell());
    return true;
  }

  return false;
}

bool TableWorkbookDocument::move_cursor_left() {
  TableSheetPtr sheet = _current_sheet;
  if (sheet->move_cursor_left()) {
    _event_sink->send_event(CURRENT_CELL_LOCATION_UPDATED,
                            sheet->current_cell());
    return true;
  }

  return false;
}

bool TableWorkbookDocument::move_cursor_right() {
  TableSheetPtr sheet = _current_sheet;
  if (sheet->move_cursor_right()) {
    _event_sink->send_event(CURRENT_CELL_LOCATION_UPDATED,
                            sheet->current_cell());
    return true;
  }

  return false;
}

std::optional<TableCellPtr>
TableWorkbookDocument::get_cell(const Location &location,
                                const std::string &sheet_name) const {
  TableSheetPtr sheet;

  if (sheet_name.empty()) {
    sheet = _current_sheet;
  } else {
    sheet = table_sheet_by_name(sheet_name);
  }

  return sheet->get_cell_by_location(location);
}

TableCellPtr TableWorkbookDocument::get_current_cell() const {
  TableSheetPtr sheet = _current_sheet;
  return sheet->get_current_cell();
}

bool TableWorkbookDocument::move_cursor_page_up() {
  TableSheetPtr sheet = _current_sheet;
  if (sheet->move_cursor_page_up()) {
    _event_sink->send_event(CURRENT_CELL_LOCATION_UPDATED,
                            sheet->current_cell());
    return true;
  }

  return false;
}

bool TableWorkbookDocument::move_cursor_page_down() {
  TableSheetPtr sheet = _current_sheet;
  if (sheet->move_cursor_page_down()) {
    _event_sink->send_event(CURRENT_CELL_LOCATION_UPDATED,
                            sheet->current_cell());
    return true;
  }

  return false;
}

Location
TableWorkbookDocument::get_cell_by_pos(const Location &position) const {
  int row = 0;
  int col = 0;

  int height = 0;
  int width = 0;

  int n;

  n = 0;
  for (const auto &rowdef : _current_sheet->row_definitions()) {
    height += rowdef->height;

    if (height > position.y()) {
      row = n;
      break;
    }

    n++;
  }

  n = 0;
  for (const auto &coldef : _current_sheet->column_definitions()) {
    width += coldef->width;

    if (width > position.x()) {
      col = n;
      break;
    }

    n++;
  }

  return Location(col, row);
}

void TableWorkbookDocument::select_cell(const Location &cell) {
  if (_current_sheet->select_cell(cell)) {
    _changed = true;
    _event_sink->send_event(CURRENT_CELL_LOCATION_UPDATED,
                            _current_sheet->current_cell());
  }
}

int TableWorkbookDocument::get_current_sheet_width() const {
  int width = 0;

  for (const auto &coldef : _current_sheet->column_definitions()) {
    width += coldef->width;
  }

  return width;
}

int TableWorkbookDocument::get_current_sheet_height() const {
  int height = 0;

  for (const auto &rowdef : _current_sheet->row_definitions()) {
    height += rowdef->height;
  }

  return height;
}

TableCellPtrVector TableWorkbookDocument::get_range(const Location &from,
                                                    const Location &to) const {
  TableCellPtrVector result;

  long min_x, min_y, max_x, max_y;

  if (from.x() > to.x()) {
    min_x = to.x();
    max_x = from.x();
  } else {
    min_x = from.x();
    max_x = to.x();
  }

  if (from.y() > to.y()) {
    min_y = to.y();
    max_y = from.y();
  } else {
    min_y = from.y();
    max_y = to.y();
  }

  for (long r = min_y; r <= max_y; r++) {
    for (long c = min_x; c <= max_x; c++) {
      const auto &opt_cell = get_cell(Location(c, r));
      if (!opt_cell) {
        continue;
      }

      result.push_back(*opt_cell);
    }
  }

  return result;
}

void TableWorkbookDocument::clear() {
  _sheets.clear();
  _current_sheet = {};
}

TableSheetPtr TableWorkbookDocument::add_sheet(const std::string &name) {
  const auto &result = std::make_shared<TableSheet>(name);
  _sheets.push_back(result);
  _changed = true;

  return result;
}

void TableWorkbookDocument::set_active_sheet(const std::string &name) {
  const auto &sheet = table_sheet_by_name(name);

  if (sheet) {
    _current_sheet = sheet;

    _changed = true;
  }
}

void TableWorkbookDocument::set_current_cell(const std::string &sheet_name,
                                             const Location &current_cell) {
  const auto &sheet = table_sheet_by_name(sheet_name);

  if (sheet) {
    sheet->select_cell(current_cell);

    _changed = true;
  }
}

void TableWorkbookDocument::clear_and_initialize() {
  clear();
  initialize();
}

void TableWorkbookDocument::initialize() {
  _sheets.push_back(std::make_shared<TableSheet>("Sheet 1"));
  //_sheets.push_back(std::make_shared<TableSheet>("Another sheet")); // TEST
  _current_sheet = _sheets[0];
  _changed = false;
}

void TableWorkbookDocument::clear_current_cell() {
  if (!_current_sheet) {
    return;
  }

  _current_sheet->clear_current_cell();

  _changed = true;

  std::any param = _current_sheet->current_cell();
  _event_sink->send_event(CELL_UPDATED, param);
}

void TableWorkbookDocument::apply_state_change_item(const StateHistoryItemPtr &state_history_item,
                                                    UpdateIdType update_id) {
  for (auto cell_state : state_history_item->cell_states) {
    update_cell_content(_current_sheet,
                        cell_state.location,
                        cell_state.prev,
                        update_id);
  }
}

void TableWorkbookDocument::undo() {
  auto state_change = _current_sheet->undo();
  UpdateIdType update_id = generate_update_id();
  apply_state_change_item(state_change, update_id);
}

void TableWorkbookDocument::redo() {
  auto state_change = _current_sheet->redo();
  UpdateIdType update_id = generate_update_id();
  apply_state_change_item(state_change, update_id);
}

TableSheetPtr
TableWorkbookDocument::find_sheet_by_name(const std::string &sheet_name) const {
  for (const auto &sheet : _sheets) {
    if (sheet->name() == sheet_name) {
      return sheet;
    }
  }

  return {};
}

bool TableWorkbookDocument::select_sheet(const TableSheetPtr &sheet) {
  if (sheet) {
    _current_sheet = sheet;
    _event_sink->send_event(SHEET_SELECTION_UPDATED, sheet->name());
    return true;
  }

  return false;
}

bool TableWorkbookDocument::select_sheet_by_name(
    const std::string &sheet_name) {
  const auto &sheet = find_sheet_by_name(sheet_name);
  return select_sheet(sheet);
}

size_t TableWorkbookDocument::get_current_column_width() const {
  return _current_sheet->get_current_column_width();
}

size_t TableWorkbookDocument::get_current_row_height() const {
  return _current_sheet->get_current_row_height();
}

void TableWorkbookDocument::set_current_column_width(size_t width) {
  _current_sheet->set_current_column_width(width);
  _changed = true;
  _event_sink->send_event(COLUMN_WIDTH_UPDATED, {});
}

void TableWorkbookDocument::set_current_row_height(size_t height) {
  _current_sheet->set_current_row_height(height);
  _changed = true;
  _event_sink->send_event(ROW_HEIGHT_UPDATED, {});
}

std::optional<Location>
TableWorkbookDocument::current_sheet_selected_cell() const {
  if (_current_sheet) {
    return _current_sheet->current_cell();
  }

  return {};
}

void TableWorkbookDocument::set_current_cell_format(
    const TableCellFormat &format) {
  if (_current_sheet) {
    _current_sheet->set_current_cell_format(format);
  }
}

std::optional<TableCellFormat>
TableWorkbookDocument::get_current_cell_format() const {
  return _current_sheet->get_current_cell_format();
}

void TableWorkbookDocument::add_update_listener(const TableCellLocation &listener,
                                                const TableCellLocation &listening_to) {
  auto it = _listeners.find(listening_to);
  if (it == _listeners.end()) {
    _listeners[listening_to] = {listener};
  } else {
    it->second.insert(listener);
  }
}

void TableWorkbookDocument::remove_current_sheet() {
  if (_sheets.size() == 1) {
    // Do not remove last sheet (for now - TODO)
    return;
  }

  auto it = std::find(_sheets.begin(), _sheets.end(), _current_sheet);
  if (it == _sheets.end()) {
    // Should never happen
    return;
  }

  _sheets.erase(it);
  _current_sheet = _sheets[0];
  _changed = true;
}

TableSearchResult
TableWorkbookDocument::search_sheets(const std::string &search_term) const {
  TableSearchResult result;

  for (const auto &sheet : _sheets) {
    LocationSet locations = sheet->search(search_term);
    for (const auto &location : locations) {
      TableSearchResultItem found_item;
      found_item.sheet = sheet;
      found_item.location = location;
      result.push_back(found_item);
    }
  }

  return result;
}

void TableWorkbookDocument::select_cell(size_t row, size_t col) {
  select_cell(Location(col, row));
}

void TableWorkbookDocument::trigger_listeners(const TableCellLocation &location,
                                              UpdateIdType update_id) {
  auto it = _listeners.find(location);
  if (it == _listeners.end()) {
    return;
  }

  std::queue<TableCellLocation> recalc_cells;

  for (const auto &listener_location : it->second) {
    recalc_cells.push(listener_location);
  }

  while (!recalc_cells.empty()) {
    TableCellLocation cell_location = recalc_cells.front();
    recalc_cells.pop();

    TableCellPtr cell = get_cell_by_location(cell_location);
    if (cell->recalc(cell_location.sheet(), update_id)) {
      // Cell content has changed. The listeners for this cell must be triggered
      // as well.
      it = _listeners.find(cell_location);
      if (it != _listeners.end()) {
        for (const auto &listener_location : it->second) {
          recalc_cells.push(listener_location);
        }
      }
    }
  }
}

TableCellPtr TableWorkbookDocument::get_cell_by_location(const TableCellLocation &location) const {
  TableCellPtr result;

  auto sheet = table_sheet_by_name(location.sheet());
  if (!sheet) {
    return {};
  }

  return sheet->get_cell_by_location(location.location());
}
