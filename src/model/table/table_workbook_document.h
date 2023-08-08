#ifndef WORKBOOK_DOCUMENT_INCLUDED
#define WORKBOOK_DOCUMENT_INCLUDED

#include "../event_sink.h"
#include "table_cell.h"
#include "table_sheet.h"
#include <string>
#include <vector>

typedef std::vector<TableCellPtr> TableCellPtrVector;

class TableWorkbookDocument {
public:
  TableWorkbookDocument(EventSink *event_sink);
  ~TableWorkbookDocument() = default;

  bool changed() const { return _changed; }

public:
  TableSheetPtr table_sheet_by_name(const std::string &name) const;
  TableSheetPtr current_sheet() const { return _current_sheet; }

  void initialize();
  void clear_and_initialize();
  void clear();
  void add_sheet(const std::string &name);
  void clear_changed_flag() { _changed = false; }
  void set_active_sheet(const std::string &name);
  void set_current_cell(const std::string &sheet_name,
                        const Location &current_cell);
  void update_cell_content(TableSheetPtr sheet, TableCellPtr cell,
                           const std::string &content);
  void clear_current_cell();

  void move_cursor_up();
  void move_cursor_down();
  void move_cursor_left();
  void move_cursor_right();
  void move_cursor_page_up();
  void move_cursor_page_down();

  Location get_cell_by_pos(const Location &position) const;
  void select_cell(const Location &cell);

  std::optional<TableCellPtr> get_cell(const Location &location) const;
  TableCellPtr get_current_cell() const;
  TableCellPtrVector get_range(const Location &from, const Location &to) const;

  void update_content_current_cell(const std::string &content);

  int get_current_sheet_width() const;
  int get_current_sheet_height() const;

  TableSheets sheets() const { return _sheets; }

  std::string file_path() const { return _path; }

  std::pair<TableRowDefinitionPtr, TableColumnDefinitionPtr>
  get_definitions_for_location(const Location &location) const {
    return _current_sheet->get_definitions_for_location(location);
  }
private:
  std::string _path;
  bool _changed;
  TableSheets _sheets;
  EventSink *_event_sink;
  TableSheetPtr _current_sheet;
};

#endif
