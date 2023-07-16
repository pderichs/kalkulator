#ifndef WORKBOOK_DOCUMENT_INCLUDED
#define WORKBOOK_DOCUMENT_INCLUDED

#include "table_cell.h"
#include "table_sheet.h"
#include <string>
#include <vector>
#include "../event_sink.h"

class TableWorkbookDocument {
public:
  TableWorkbookDocument(EventSink *event_sink);
  ~TableWorkbookDocument() = default;

  bool changed() const { return _changed; }

public:
  TableSheetPtr table_sheet_by_name(const std::string &name) const;
  TableSheetPtr current_sheet() const { return _current_sheet; }

  void move_cursor_up();
  void move_cursor_down();
  void move_cursor_left();
  void move_cursor_right();

  std::optional<TableCellPtr> get_cell(const Location& location);
  TableCellPtr get_current_cell() const;

  void update_content_current_cell(const std::string& content);

private:
  std::string _path;
  bool _changed;
  TableSheets _sheets;
  EventSink *_event_sink;
  TableSheetPtr _current_sheet;
};

#endif
