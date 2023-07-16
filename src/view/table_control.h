#ifndef TABLECONTROL_INCLUDED
#define TABLECONTROL_INCLUDED

#include "../model/table/workbook_document.h"
#include "../model/table/table_sheet.h"
#include <wx/wx.h>

#include "event_sink.h"
#include "location.h"

class TableControl : public wxScrolledWindow {
public:
  TableControl(WorkbookDocument* document, EventSink *event_sink, wxWindow *parent, wxWindowID id = wxID_ANY,
               const wxPoint &pos = wxDefaultPosition,
               const wxSize &size = wxDefaultSize,
               long style = wxTAB_TRAVERSAL);

  virtual ~TableControl();

  void OnDraw(wxDC &dc);
  void OnScroll(wxScrollWinEvent &scrollEvent);
  void OnKeyPress(wxKeyEvent &event);

  void RefreshScrollbars();

  void DrawTable(wxDC *dc, TableSheetPtr sheet);

  Location GetScrollPosition() const;

private:
  void DrawHeaders(wxDC *dc, const Location &scrollPos, int width, int height, TableSheetPtr sheet);
  void DrawCells(wxDC *dc, const Location &scrollPos, int width, int height, TableSheetPtr sheet);
  wxRect GetCellRectByLocation(const Location& cell);
  wxRect GetCurrentScrollArea() const;
  void UpdateSheet() {
    auto sheet = _document->current_sheet();
    RefreshScrollbars();
    Refresh();
  }

private:
  WorkbookDocument* _document;
  wxPen* _caption_grid_pen;
  wxPen *_grid_pen;
  wxPen *_current_cell_pen;
  EventSink *_event_sink;
};

#endif
