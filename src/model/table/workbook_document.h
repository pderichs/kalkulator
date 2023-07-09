#ifndef WORKBOOK_DOCUMENT_INCLUDED
#define WORKBOOK_DOCUMENT_INCLUDED

#include "table_sheet.h"
#include <string>
#include <vector>

class WorkbookDocument {
public:
  WorkbookDocument();
  ~WorkbookDocument();

  bool changed() const { return _changed; }
  TableSheetPtr first_sheet() const {
    if (_sheets.empty()) {
      return {};
    }

    return _sheets[0];
  }

private:
  TableSheetPtr table_sheet_by_name(const std::string &name) const;

private:
  std::string _path;
  bool _changed;
  TableSheets _sheets;
};

#endif
