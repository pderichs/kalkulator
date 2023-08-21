#include "table_row_headers_control.h"
#include "kalkulator_system_colors.h"

//  TODO: Remove these as soon as possible!
const int ROW_HEADER_WIDTH = 50;
// const int COLUMN_HEADER_HEIGHT = 30;

// NOTE Vertical scrolling is disabled by style
TableRowHeadersControl::TableRowHeadersControl(
    KalkulatorSystemColorsPtr sys_colors, TableWorkbookDocumentPtr document,
    EventSink *event_sink, wxWindow *parent, wxWindowID id, const wxPoint &pos,
    const wxSize &size, long style)
    : TableSheetView(document, event_sink, parent, id, pos, size,
                     style | ~wxVSCROLL) {
  _sys_colors = sys_colors;

  int height = document->get_current_sheet_height();
  SetScrollRate(0, 10);
  SetVirtualSize(ROW_HEADER_WIDTH, height);
  EnableScrolling(false, false);
}

void TableRowHeadersControl::OnDraw(wxDC &dc) {
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

void TableRowHeadersControl::DrawHeaders(wxDC *dc, const Location &scrollPos,
                                         int width, int height,
                                         TableSheetPtr sheet) {
  std::ignore = width;
  std::ignore = height;
  std::ignore = scrollPos;

  int y, c;

  // Set pen and brushes for headers of columns and rows
  dc->SetPen(*_sys_colors->caption_grid_pen);
  dc->SetBrush(*_sys_colors->caption_background_brush);

  // Rows
  c = 0;
  y = 2;
  for (auto rowdef : sheet->row_definitions) {
    // if (y > height) {
    //   break;
    // }

    auto name = rowdef->caption;

    if (name.empty()) {
      std::stringstream ss;
      ss << c;
      name = ss.str();
    }

    wxRect rect(0, y, ROW_HEADER_WIDTH, rowdef->height);
    dc->DrawRectangle(rect);
    DrawTextInCenter(dc, name, rect);

    y += rowdef->height;

    c++;
  }
}

wxSize TableRowHeadersControl::DoGetBestSize() const {
  return wxSize(ROW_HEADER_WIDTH, 100);
}
