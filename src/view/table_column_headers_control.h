#ifndef TABLE_COLUMN_HEADERS_CONTROL_INCLUDED
#define TABLE_COLUMN_HEADERS_CONTROL_INCLUDED

#include "kalkulator_system_colors.h"
#include "table_sheet_view.h"

class TableColumnHeadersControl : public TableSheetView {
public:
  TableColumnHeadersControl(KalkulatorSystemColorsPtr sys_colors,
                            TableWorkbookDocumentPtr document,
                            EventSink *event_sink, wxWindow *parent,
                            wxWindowID id = wxID_ANY,
                            const wxPoint &pos = wxDefaultPosition,
                            const wxSize &size = wxDefaultSize,
                            long style = wxTAB_TRAVERSAL);
  virtual ~TableColumnHeadersControl() = default;

  void OnDraw(wxDC &dc);
  virtual wxSize DoGetBestSize() const;

private:
  void DrawHeaders(wxDC *dc, const Location &scrollPos, int width, int height,
                   TableSheetPtr sheet);

private:
  KalkulatorSystemColorsPtr _sys_colors;
};

#endif
