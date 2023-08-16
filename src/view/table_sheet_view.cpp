#include "table_sheet_view.h"

TableSheetView::TableSheetView(TableWorkbookDocument *document,
                               EventSink *event_sink, wxWindow *parent,
                               wxWindowID id, const wxPoint &pos,
                               const wxSize &size, long style)
    : wxScrolledWindow(parent, id, pos, size, style), _document(document),
      _event_sink(event_sink) {}

Location TableSheetView::GetScrollPosition() const {
  return Location(GetScrollPos(wxHORIZONTAL), GetScrollPos(wxVERTICAL));
}

void TableSheetView::DrawTextInCenter(wxDC *dc, const wxString &s,
                                      const wxRect &rect) {
  // Calculate the center coordinates of the wxRect
  int centerX = rect.GetX() + rect.GetWidth() / 2;
  int centerY = rect.GetY() + rect.GetHeight() / 2;

  // Draw the text at the center coordinates
  int textWidth, textHeight;
  dc->GetTextExtent(s, &textWidth, &textHeight);

  int textX = centerX - textWidth / 2;
  int textY = centerY - textHeight / 2;
  dc->DrawText(s, textX, textY);
}
