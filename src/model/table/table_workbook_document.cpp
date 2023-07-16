#include "table_workbook_document.h"
#include "table_cell.h"
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
  TableSheetPtr sheet = current_sheet();
  TableCellPtr cell = sheet->get_current_cell();

  assert(cell);

  cell->update_content(content);

  // TODO
  // std::any param = std::pair<int, int>(sheet->current_cell.x(), sheet->current_cell.y());
  // _event_sink->send_event(CELL_UPDATED, param);
}
