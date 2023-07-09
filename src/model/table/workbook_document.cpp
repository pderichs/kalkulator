#include "workbook_document.h"
#include "table_sheet.h"
#include <memory>

WorkbookDocument::WorkbookDocument() {
  _sheets.push_back(std::make_shared<TableSheet>("Sheet 1"));
}

WorkbookDocument::~WorkbookDocument() {
}
