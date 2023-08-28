#ifndef TABLE_ROW_HEADERS_CONTROL_INCLUDED
#define TABLE_ROW_HEADERS_CONTROL_INCLUDED

#include "kalkulator_system_colors.h"
#include "table_sheet_view.h"
#include <wx/wx.h>

class TableRowHeadersControl : public TableSheetView {
public:
  TableRowHeadersControl(KalkulatorSystemColorsPtr sys_colors,
                         TableWorkbookDocumentPtr document,
                         EventSink *event_sink, wxWindow *parent,
                         wxWindowID id = wxID_ANY,
                         const wxPoint &pos = wxDefaultPosition,
                         const wxSize &size = wxDefaultSize,
                         long style = wxTAB_TRAVERSAL);
  virtual ~TableRowHeadersControl() = default;

  void OnDraw(wxDC &dc);
  virtual wxSize DoGetBestSize() const;
  void OnFocus(wxFocusEvent& WXUNUSED(event));

private:
  void DrawHeaders(wxDC *dc, const Location &scrollPos, int width, int height,
                   TableSheetPtr sheet);

private:
  KalkulatorSystemColorsPtr _sys_colors;
};

#endif
