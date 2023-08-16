#ifndef TABLE_COLUMN_HEADERS_CONTROL_INCLUDED
#define TABLE_COLUMN_HEADERS_CONTROL_INCLUDED

#include "../model/event_sink.h"
#include "../model/table/table_workbook_document.h"
#include "kalkulator_system_colors.h"
#include "tools.h"
#include <wx/wx.h>

class TableColumnHeadersControl : public wxScrolledWindow {
public:
  TableColumnHeadersControl(KalkulatorSystemColorsPtr sys_colors,
                            TableWorkbookDocument *document,
                            EventSink *event_sink, wxWindow *parent,
                            wxWindowID id = wxID_ANY,
                            const wxPoint &pos = wxDefaultPosition,
                            const wxSize &size = wxDefaultSize,
                            long style = wxTAB_TRAVERSAL);
  virtual ~TableColumnHeadersControl() = default;

  void OnDraw(wxDC &dc);

  Location GetScrollPosition() const;

  void DrawTextInCenter(wxDC *dc, const wxString &s, const wxRect &rect);
  virtual wxSize DoGetBestSize() const;

private:
  void DrawHeaders(wxDC *dc, const Location &scrollPos, int width, int height,
                   TableSheetPtr sheet);

private:
  TableWorkbookDocument *_document;
  EventSink *_event_sink;
  KalkulatorSystemColorsPtr _sys_colors;
};

#endif
