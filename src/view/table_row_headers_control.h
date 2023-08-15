#ifndef TABLE_ROW_HEADERS_CONTROL_INCLUDED
#define TABLE_ROW_HEADERS_CONTROL_INCLUDED
#include "../model/event_sink.h"
#include "../model/table/table_workbook_document.h"
#include "tools.h"
#include <wx/wx.h>

class TableRowHeadersControl : public wxScrolledWindow {
public:
  TableRowHeadersControl(TableWorkbookDocument *document, EventSink *event_sink,
                         wxWindow *parent, wxWindowID id = wxID_ANY,
                         const wxPoint &pos = wxDefaultPosition,
                         const wxSize &size = wxDefaultSize,
                         long style = wxTAB_TRAVERSAL);
  virtual ~TableRowHeadersControl();

  void OnDraw(wxDC &dc);

  Location GetScrollPosition() const;

  void DrawTextInCenter(wxDC *dc, const wxString &s, const wxRect &rect);

private:
  void DrawHeaders(wxDC *dc, const Location &scrollPos, int width, int height,
                   TableSheetPtr sheet);

private:
  TableWorkbookDocument *_document;
  EventSink *_event_sink;
  // TODO: Move Pens and Brushes to a more central position?
  wxPen *_caption_grid_pen;
  wxBrush *_caption_background_brush;
};

#endif
