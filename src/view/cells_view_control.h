#ifndef CELLS_VIEW_CONTROL_INCLUDED
#define CELLS_VIEW_CONTROL_INCLUDED

#include "../model/table/table_cell_orientation.h"
#include "kalkulator_system_colors.h"
#include "table_sheet_view.h"

class CellsViewControl : public TableSheetView {
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

  void OnCellUpdate(const Location &location);

  virtual void ScrollWindow(int dx, int dy, const wxRect *rect);

private:
  void DrawCells(wxDC *dc, const Location &scrollPos, int width, int height,
                 TableSheetPtr sheet);
  wxRect GetCellRectByLocation(const Location &cell);
  wxRect GetCurrentScrollArea() const;

  void ScrollToCurrentCell();
  Location GetTableCellByClickPosition(const wxPoint &pos) const;
  void ScrollToCell(const Location &cell, TableCellOrientation orientation);

private:
  KalkulatorSystemColorsPtr _sys_colors;
};

#endif
