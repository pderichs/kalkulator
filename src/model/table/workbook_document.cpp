#include "workbook_document.h"
#include "table_sheet.h"
#include <memory>

WorkbookDocument::WorkbookDocument() {
  _sheets.push_back(std::make_shared<TableSheet>("Sheet 1"));
}

WorkbookDocument::~WorkbookDocument() {
}

TableSheetPtr WorkbookDocument::table_sheet_by_name(const std::string& name) const {
  for (auto sheet: _sheets) {
    if (sheet->name() == name) {
      return sheet;
    }
  }

  return {};
}
