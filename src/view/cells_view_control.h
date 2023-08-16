#ifndef CELLS_VIEW_CONTROL_INCLUDED
#define CELLS_VIEW_CONTROL_INCLUDED

#include "../model/table/table_cell_orientation.h"
#include "../model/table/table_sheet.h"
#include "../model/table/table_workbook_document.h"
#include <wx/wx.h>

#include "../model/event_sink.h"
#include "kalkulator_system_colors.h"
#include "location.h"

class CellsViewControl : public wxScrolledWindow {
public:
  CellsViewControl(KalkulatorSystemColorsPtr sys_colors,
                   TableWorkbookDocument *document, EventSink *event_sink,
                   wxWindow *parent, wxWindowID id = wxID_ANY,
                   const wxPoint &pos = wxDefaultPosition,
                   const wxSize &size = wxDefaultSize,
                   long style = wxTAB_TRAVERSAL);

  virtual ~CellsViewControl() = default;

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

  virtual void ScrollWindow(int dx, int dy, const wxRect *rect);

private:
  // void DrawHeaders(wxDC *dc, const Location &scrollPos, int width, int
  // height,
  //                  TableSheetPtr sheet);
  void DrawCells(wxDC *dc, const Location &scrollPos, int width, int height,
                 TableSheetPtr sheet);
  wxRect GetCellRectByLocation(const Location &cell);
  wxRect GetCurrentScrollArea() const;

  void ScrollToCurrentCell();
  void DrawTextInCenter(wxDC *dc, const wxString &s, const wxRect &rect);
  Location GetTableCellByClickPosition(const wxPoint &pos) const;
  void ScrollToCell(const Location &cell, TableCellOrientation orientation);

private:
  TableWorkbookDocument *_document;
  EventSink *_event_sink;

  KalkulatorSystemColorsPtr _sys_colors;
};

#endif
