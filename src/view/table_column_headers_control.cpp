#include "table_column_headers_control.h"

//  FIXME: Remove these as soon as possible!
const int ROW_HEADER_WIDTH = 50;
const int COLUMN_HEADER_HEIGHT = 30;

TableColumnHeadersControl::TableColumnHeadersControl(
    KalkulatorSystemColorsPtr sys_colors, TableWorkbookDocument *document,
    EventSink *event_sink, wxWindow *parent, wxWindowID id, const wxPoint &pos,
    const wxSize &size, long style)
    : wxScrolledWindow(parent, id, pos, size, style | ~wxHSCROLL) {
  _document = document;
  _event_sink = event_sink;

  _sys_colors = sys_colors;

  int width = _document->get_current_sheet_width() + ROW_HEADER_WIDTH;
  SetScrollRate(10, 0);
  SetVirtualSize(width, COLUMN_HEADER_HEIGHT);
  EnableScrolling(false, false);
}

void TableColumnHeadersControl::OnDraw(wxDC &dc) {
  TableSheetPtr sheet = _document->current_sheet();
  if (!sheet) {
    // TODO Clear?
    return;
  }

  Location scrollPos = GetScrollPosition();
  int width;
  int height;
  GetSize(&width, &height);
  DrawHeaders(&dc, scrollPos, width, height, sheet);
}

void TableColumnHeadersControl::DrawHeaders(wxDC *dc, const Location &scrollPos,
                                            int width, int height,
                                            TableSheetPtr sheet) {
  std::ignore = width;
  std::ignore = height;
  std::ignore = scrollPos;

  int x, c;

  // Set pen and brushes for headers of columns and rows
  dc->SetPen(*_sys_colors->caption_grid_pen);
  dc->SetBrush(*_sys_colors->caption_background_brush);

  // TODO for now we are drawing all available columns and rows
  // this can possibly be optimized

  // Columns
  c = 0;
  x = ROW_HEADER_WIDTH;
  for (auto coldef : sheet->column_definitions) {
    // if (x > width) {
    //   break;
    // }

    auto name = coldef->caption;

    // TODO Move to tools
    if (name.empty()) {
      std::stringstream ss;
      ss << c;
      name = ss.str();
    }

    wxRect rect(x, 2, coldef->width, COLUMN_HEADER_HEIGHT);
    dc->DrawRectangle(rect);
    DrawTextInCenter(dc, name, rect);

    x += coldef->width;

    c++;
  }
}

// TODO: Remove duplicated code
Location TableColumnHeadersControl::GetScrollPosition() const {
  return Location(GetScrollPos(wxHORIZONTAL), GetScrollPos(wxVERTICAL));
}

// TODO: Remove duplicated code
void TableColumnHeadersControl::DrawTextInCenter(wxDC *dc, const wxString &s,
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

wxSize TableColumnHeadersControl::DoGetBestSize() const {
  return wxSize(100, COLUMN_HEADER_HEIGHT);
}
