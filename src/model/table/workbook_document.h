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

private:
  std::string _path;
  bool _changed;
  TableSheets _sheets;
};

#endif
