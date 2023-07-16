#include "workbook_document.h"
#include "table_sheet.h"
#include <memory>

WorkbookDocument::WorkbookDocument(EventSink *event_sink) {
  _event_sink = event_sink;

  _sheets.push_back(std::make_shared<TableSheet>("Sheet 1"));

  _current_sheet = _sheets[0];
}

WorkbookDocument::~WorkbookDocument() {
}

TableSheetPtr WorkbookDocument::table_sheet_by_name(const std::string& name) const {
  for (auto sheet: _sheets) {
    if (sheet->name == name) {
      return sheet;
    }
  }

  return {};
}
