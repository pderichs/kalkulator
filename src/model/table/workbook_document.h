#ifndef WORKBOOK_DOCUMENT_INCLUDED
#define WORKBOOK_DOCUMENT_INCLUDED

#include "table_sheet.h"
#include <string>
#include <vector>
#include "event_sink.h"

class WorkbookDocument {
public:
  WorkbookDocument(EventSink *event_sink);
  ~WorkbookDocument() = default;

  bool changed() const { return _changed; }

public:
  TableSheetPtr table_sheet_by_name(const std::string &name) const;
  TableSheetPtr current_sheet() const { return _current_sheet; }

private:
  std::string _path;
  bool _changed;
  TableSheets _sheets;
  EventSink *_event_sink;
  TableSheetPtr _current_sheet;
};

#endif
