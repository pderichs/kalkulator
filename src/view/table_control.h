#ifndef TABLECONTROL_INCLUDED
#define TABLECONTROL_INCLUDED

#include "../model/table/table_sheet.h"
#include <wx/wx.h>

class TableControl : public wxScrolledWindow {
public:
  TableControl(wxWindow *parent, wxWindowID id = wxID_ANY,
               const wxPoint &pos = wxDefaultPosition,
               const wxSize &size = wxDefaultSize,
               long style = wxTAB_TRAVERSAL);

  void OnDraw(wxDC &dc);
  void OnScroll(wxScrollWinEvent &scrollEvent);

  void RefreshScrollbars();

  void SetSheet(TableSheetPtr sheet) {
    _sheet = sheet;
    RefreshScrollbars();
    Refresh();
  }
  void DrawGrid(wxDC *dc);

private:
  TableSheetPtr _sheet;
};

#endif
