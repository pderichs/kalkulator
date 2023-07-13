#ifndef TABLECONTROL_INCLUDED
#define TABLECONTROL_INCLUDED

#include "../model/table/table_sheet.h"
#include <wx/wx.h>

#include "location.h"

class TableControl : public wxScrolledWindow {
public:
  TableControl(wxWindow *parent, wxWindowID id = wxID_ANY,
               const wxPoint &pos = wxDefaultPosition,
               const wxSize &size = wxDefaultSize,
               long style = wxTAB_TRAVERSAL);

  virtual ~TableControl();

  void OnDraw(wxDC &dc);
  void OnScroll(wxScrollWinEvent &scrollEvent);

  void RefreshScrollbars();

  void SetSheet(TableSheetPtr sheet) {
    _sheet = sheet;
    RefreshScrollbars();
    Refresh();
  }
  void DrawGrid(wxDC *dc);

  Location GetScrollPosition() const;

private:
  void DrawHeaders(wxDC *dc, const Location &scrollPos, int width, int height);
  void DrawCells(wxDC *dc, const Location &scrollPos, int width, int height);

private:
  TableSheetPtr _sheet;
  wxPen* _caption_grid_pen;
  wxPen *_grid_pen;
  wxPen *_current_cell_pen;
};

#endif
