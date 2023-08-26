#include "table_workbook_document.h"
#include "table_cell.h"
#include "table_column_definition.h"
#include "table_row_definition.h"
#include "table_sheet.h"
#include <algorithm>
#include <cassert>
#include <memory>
#include <tuple>
#include <utility>

TableWorkbookDocument::TableWorkbookDocument(EventSink *event_sink) {
  _event_sink = event_sink;

  initialize();
}

TableSheetPtr
TableWorkbookDocument::table_sheet_by_name(const std::string &name) const {
  for (auto sheet : _sheets) {
    if (sheet->name == name) {
      return sheet;
    }
  }

  return {};
}

void TableWorkbookDocument::update_cell_content(TableSheetPtr sheet,
                                                Location cell_location,
                                                const std::string &content) {
  sheet->update_content(cell_location, content);

  _changed = true;

  std::any param = cell_location;
  _event_sink->send_event(CELL_UPDATED, param);
}

void TableWorkbookDocument::update_content_current_cell(
    const std::string &content) {
  TableSheetPtr sheet = _current_sheet;

  update_cell_content(sheet, sheet->current_cell, content);
}

bool TableWorkbookDocument::move_cursor_up() {
  TableSheetPtr sheet = _current_sheet;
  if (sheet->move_cursor_up()) {
    _event_sink->send_event(CURRENT_CELL_LOCATION_UPDATED, sheet->current_cell);
    return true;
  }

  return false;
}

bool TableWorkbookDocument::move_cursor_down() {
  TableSheetPtr sheet = _current_sheet;
  if (sheet->move_cursor_down()) {
    _event_sink->send_event(CURRENT_CELL_LOCATION_UPDATED, sheet->current_cell);
    return true;
  }

  return false;
}

bool TableWorkbookDocument::move_cursor_left() {
  TableSheetPtr sheet = _current_sheet;
  if (sheet->move_cursor_left()) {
    _event_sink->send_event(CURRENT_CELL_LOCATION_UPDATED, sheet->current_cell);
    return true;
  }

  return false;
}

bool TableWorkbookDocument::move_cursor_right() {
  TableSheetPtr sheet = _current_sheet;
  if (sheet->move_cursor_right()) {
    _event_sink->send_event(CURRENT_CELL_LOCATION_UPDATED, sheet->current_cell);
    return true;
  }

  return false;
}

std::optional<TableCellPtr>
TableWorkbookDocument::get_cell(const Location &location) const {
  TableSheetPtr sheet = _current_sheet;
  return sheet->get_cell_by_location(location);
}

TableCellPtr TableWorkbookDocument::get_current_cell() const {
  TableSheetPtr sheet = _current_sheet;
  return sheet->get_current_cell();
}

bool TableWorkbookDocument::move_cursor_page_up() {
  TableSheetPtr sheet = _current_sheet;
  if (sheet->move_cursor_page_up()) {
    _event_sink->send_event(CURRENT_CELL_LOCATION_UPDATED, sheet->current_cell);
    return true;
  }

  return false;
}

bool TableWorkbookDocument::move_cursor_page_down() {
  TableSheetPtr sheet = _current_sheet;
  if (sheet->move_cursor_page_down()) {
    _event_sink->send_event(CURRENT_CELL_LOCATION_UPDATED, sheet->current_cell);
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
  for (const auto &rowdef : _current_sheet->row_definitions) {
    height += rowdef->height;

    if (height > position.y()) {
      row = n;
      break;
    }

    n++;
  }

  n = 0;
  for (const auto &coldef : _current_sheet->column_definitions) {
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
                            _current_sheet->current_cell);
  }
}

int TableWorkbookDocument::get_current_sheet_width() const {
  int width = 0;

  for (const auto &coldef : _current_sheet->column_definitions) {
    width += coldef->width;
  }

  return width;
}

int TableWorkbookDocument::get_current_sheet_height() const {
  int height = 0;

  for (const auto &rowdef : _current_sheet->row_definitions) {
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
  const auto& result = std::make_shared<TableSheet>(name);
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

  std::any param = _current_sheet->current_cell;
  _event_sink->send_event(CELL_UPDATED, param);
}

void TableWorkbookDocument::undo() { _current_sheet->undo(); }

void TableWorkbookDocument::redo() { _current_sheet->redo(); }

TableSheetPtr
TableWorkbookDocument::find_sheet_by_name(const std::string &sheet_name) const {
  for (const auto &sheet : _sheets) {
    if (sheet->name == sheet_name) {
      return sheet;
    }
  }

  return {};
}

bool TableWorkbookDocument::select_sheet_by_name(
    const std::string &sheet_name) {
  const auto &sheet = find_sheet_by_name(sheet_name);
  if (sheet) {
    _current_sheet = sheet;
    _event_sink->send_event(SHEET_SELECTION_UPDATED, sheet_name);
    return true;
  }

  return false;
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

std::optional<Location> TableWorkbookDocument::current_sheet_selected_cell() const {
  if (_current_sheet) {
    return _current_sheet->current_cell;
  }

  return {};
}
