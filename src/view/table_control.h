#ifndef TABLECONTROL_INCLUDED
#define TABLECONTROL_INCLUDED

#include "../model/table/table_sheet.h"
#include "../model/table/table_workbook_document.h"
#include "../model/table/table_cell_orientation.h"
#include <wx/wx.h>

#include "../model/event_sink.h"
#include "location.h"

class TableControl : public wxScrolledWindow {
public:
  TableControl(TableWorkbookDocument *document, EventSink *event_sink,
               wxWindow *parent, wxWindowID id = wxID_ANY,
               const wxPoint &pos = wxDefaultPosition,
               const wxSize &size = wxDefaultSize,
               long style = wxTAB_TRAVERSAL);

  virtual ~TableControl();

  void OnDraw(wxDC &dc);
  void OnKeyPress(wxKeyEvent &event);
  void OnLeftDown(wxMouseEvent &event);
  void OnCopy();
  void OnPaste();
  void OnCut();

  void RefreshScrollbars();

  void DrawTable(wxDC *dc, TableSheetPtr sheet);

  Location GetScrollPosition() const;

  void OnCellUpdate(const Location &location);

private:
  void DrawHeaders(wxDC *dc, const Location &scrollPos, int width, int height,
                   TableSheetPtr sheet);
  void DrawCells(wxDC *dc, const Location &scrollPos, int width, int height,
                 TableSheetPtr sheet);
  wxRect GetCellRectByLocation(const Location &cell);
  wxRect GetCurrentScrollArea() const;
  void UpdateSheet() {
    // auto sheet = _document->current_sheet();
    // RefreshScrollbars();
    // Refresh();
  }

  void ScrollToCurrentCell();
  void DrawTextInCenter(wxDC *dc, const wxString &s, const wxRect &rect);
  Location GetTableCellByClickPosition(const wxPoint &pos) const;
  void ScrollToCell(const Location& cell, TableCellOrientation orientation);

private:
  TableWorkbookDocument *_document;
  wxPen *_caption_grid_pen;
  wxPen *_grid_pen;
  wxPen *_current_cell_pen;
  EventSink *_event_sink;
  wxBrush *_window_brush;
  wxColour _window_color;
  wxColour _window_text_color;
  wxColour _button_face_color;
  wxColour _button_text_color;
  wxBrush *_caption_background_brush;
  wxColour _active_border_color;
};

#endif
