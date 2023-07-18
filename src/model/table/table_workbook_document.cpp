#include "table_workbook_document.h"
#include "table_cell.h"
#include "table_column_definition.h"
#include "table_row_definition.h"
#include "table_sheet.h"
#include <memory>
#include <tuple>
#include <utility>
#include <cassert>

TableWorkbookDocument::TableWorkbookDocument(EventSink *event_sink) {
  _event_sink = event_sink;
  _sheets.push_back(std::make_shared<TableSheet>("Sheet 1"));
  _current_sheet = _sheets[0];
  _changed = false;
}

TableSheetPtr TableWorkbookDocument::table_sheet_by_name(const std::string& name) const {
  for (auto sheet: _sheets) {
    if (sheet->name == name) {
      return sheet;
    }
  }

  return {};
}

void TableWorkbookDocument::update_content_current_cell(const std::string& content) {
  TableSheetPtr sheet = _current_sheet;
  TableCellPtr cell = sheet->get_current_cell();

  assert(cell);

  cell->update_content(content);

  std::any param = sheet->current_cell;
  _event_sink->send_event(CELL_UPDATED, param);
}

void TableWorkbookDocument::move_cursor_up() {
  TableSheetPtr sheet = _current_sheet;
  if (sheet->move_cursor_up()) {
    _event_sink->send_event(CURRENT_CELL_LOCATION_UPDATED, sheet->current_cell);
  }
}

void TableWorkbookDocument::move_cursor_down() {
  TableSheetPtr sheet = _current_sheet;
  if (sheet->move_cursor_down()) {
    _event_sink->send_event(CURRENT_CELL_LOCATION_UPDATED, sheet->current_cell);
  }
}

void TableWorkbookDocument::move_cursor_left() {
  TableSheetPtr sheet = _current_sheet;
  if (sheet->move_cursor_left()) {
    _event_sink->send_event(CURRENT_CELL_LOCATION_UPDATED, sheet->current_cell);
  }
}

void TableWorkbookDocument::move_cursor_right() {
  TableSheetPtr sheet = _current_sheet;
  if (sheet->move_cursor_right()) {
    _event_sink->send_event(CURRENT_CELL_LOCATION_UPDATED, sheet->current_cell);
  }
}

std::optional<TableCellPtr> TableWorkbookDocument::get_cell(const Location& location) {
  TableSheetPtr sheet = _current_sheet;
  return sheet->get_cell_by_location(location);
}

TableCellPtr TableWorkbookDocument::get_current_cell() const {
  TableSheetPtr sheet = _current_sheet;
  return sheet->get_current_cell();
}

void TableWorkbookDocument::move_cursor_page_up() {
  TableSheetPtr sheet = _current_sheet;
  if (sheet->move_cursor_page_up()) {
    _event_sink->send_event(CURRENT_CELL_LOCATION_UPDATED, sheet->current_cell);
  }
}

void TableWorkbookDocument::move_cursor_page_down() {
  TableSheetPtr sheet = _current_sheet;
  if (sheet->move_cursor_page_down()) {
    _event_sink->send_event(CURRENT_CELL_LOCATION_UPDATED, sheet->current_cell);
  }
}

Location TableWorkbookDocument::get_cell_by_pos(const Location& position) const {
  int row = 0;
  int col = 0;

  int height = 0;
  int width = 0;

  int n;

  n = 0;
  for (const auto& rowdef : _current_sheet->row_definitions) {
    height += rowdef->height;

    if (height > position.y()) {
      row = n;
      break;
    }

    n++;
  }

  n = 0;
  for (const auto& coldef : _current_sheet->column_definitions) {
    width += coldef->width;

    if (width > position.x()) {
      col = n;
      break;
    }

    n++;
  }

  return Location(col, row);
}

void TableWorkbookDocument::select_cell(const Location& cell) {
  if (_current_sheet->select_cell(cell)) {
    _event_sink->send_event(CURRENT_CELL_LOCATION_UPDATED, _current_sheet->current_cell);
  }
}

int TableWorkbookDocument::get_current_sheet_width() const {
  int width = 0;

  for (const auto& coldef : _current_sheet->column_definitions) {
    width += coldef->width;
  }

  return width;
}

int TableWorkbookDocument::get_current_sheet_height() const {
  int height = 0;

  for (const auto& rowdef : _current_sheet->row_definitions) {
    height += rowdef->height;
  }

  return height;
}
