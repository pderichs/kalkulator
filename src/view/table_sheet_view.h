#ifndef TABLE_SHEET_VIEW_INCLUDED
#define TABLE_SHEET_VIEW_INCLUDED

#include "../model/table/table_workbook_document.h"
#include "tools.h"
#include <wx/wx.h>

class TableSheetView : public wxScrolledWindow {
public:
  TableSheetView(TableWorkbookDocumentPtr document, EventSink *event_sink,
                 wxWindow *parent, wxWindowID id, const wxPoint &pos,
                 const wxSize &size, long style);
  virtual ~TableSheetView() = default;

  Location GetScrollPosition() const;
  void DrawTextInCenter(wxDC *dc, const wxString &s, const wxRect &rect);

protected:
  TableWorkbookDocumentPtr _document;
  EventSink *_event_sink;
};

#endif
