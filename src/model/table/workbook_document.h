#ifndef WORKBOOK_DOCUMENT_INCLUDED
#define WORKBOOK_DOCUMENT_INCLUDED

#include <string>

class WorkbookDocument {
public:
  bool changed() const { return _changed; }

private:
  std::string _path;
  bool _changed;
};

#endif
