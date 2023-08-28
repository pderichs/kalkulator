#ifndef WORKBOOK_DOCUMENT_INCLUDED
#define WORKBOOK_DOCUMENT_INCLUDED

#include "../event_sink.h"
#include "table_cell.h"
#include "table_cell_format.h"
#include "table_sheet.h"
#include <memory>
#include <string>
#include <vector>

typedef std::vector<TableCellPtr> TableCellPtrVector;

class TableWorkbookDocument {
public:
  TableWorkbookDocument(EventSink *event_sink);
  ~TableWorkbookDocument() = default;

  bool changed() const { return _changed; }

  TableSheetPtr table_sheet_by_name(const std::string &name) const;
  TableSheetPtr current_sheet() const { return _current_sheet; }

  void initialize();
  void clear_and_initialize();
  void clear();
  TableSheetPtr add_sheet(const std::string &name);
  void clear_changed_flag() { _changed = false; }
  void set_active_sheet(const std::string &name);
  void set_current_cell(const std::string &sheet_name,
                        const Location &current_cell);
  void update_cell_content(TableSheetPtr sheet, Location cell_location,
                           const std::string &content);
  void clear_current_cell();

  bool move_cursor_up();
  bool move_cursor_down();
  bool move_cursor_left();
  bool move_cursor_right();
  bool move_cursor_page_up();
  bool move_cursor_page_down();

  std::optional<Location> current_sheet_selected_cell() const;

  Location get_cell_by_pos(const Location &position) const;
  void select_cell(const Location &cell);

  bool select_sheet_by_name(const std::string &sheet_name);
  TableSheetPtr find_sheet_by_name(const std::string &sheet_name) const;

  std::optional<TableCellPtr> get_cell(const Location &location) const;
  TableCellPtr get_current_cell() const;
  TableCellPtrVector get_range(const Location &from, const Location &to) const;

  void update_content_current_cell(const std::string &content);

  int get_current_sheet_width() const;
  int get_current_sheet_height() const;

  size_t get_current_column_width() const;
  size_t get_current_row_height() const;

  void set_current_column_width(size_t width);
  void set_current_row_height(size_t height);

  TableSheets sheets() const { return _sheets; }

  void set_file_path(const std::string &path) { _path = path; }
  std::string file_path() const { return _path; }

  std::pair<TableRowDefinitionPtr, TableColumnDefinitionPtr>
  get_definitions_for_location(const Location &location) const {
    return _current_sheet->get_definitions_for_location(location);
  }

  void undo();
  void redo();

  void set_current_cell_format(const TableCellFormat& format);

private:
  std::string _path;
  bool _changed;
  TableSheets _sheets;
  EventSink *_event_sink;
  TableSheetPtr _current_sheet;
};

typedef std::shared_ptr<TableWorkbookDocument> TableWorkbookDocumentPtr;

#endif
