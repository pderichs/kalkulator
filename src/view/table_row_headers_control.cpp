#include "table_row_headers_control.h"

//  FIXME: Remove these as soon as possible!
const int ROW_HEADER_WIDTH = 50;
// const int COLUMN_HEADER_HEIGHT = 30;

TableRowHeadersControl::TableRowHeadersControl(TableWorkbookDocument *document,
                                               EventSink *event_sink,
                                               wxWindow *parent, wxWindowID id,
                                               const wxPoint &pos,
                                               const wxSize &size, long style)
    : wxScrolledWindow(parent, id, pos, size, style) {
  _document = document;
  _event_sink = event_sink;

  auto button_text_color =
      wxSystemSettingsNative::GetColour(wxSYS_COLOUR_BTNTEXT);
  auto button_face_color =
      wxSystemSettingsNative::GetColour(wxSYS_COLOUR_BTNFACE);
  _caption_background_brush = new wxBrush(button_face_color);
  _caption_grid_pen = new wxPen(button_text_color);

  // FIXME: Hide scroll bars
  int height = document->get_current_sheet_height();
  SetScrollRate(0, 10);
  SetVirtualSize(ROW_HEADER_WIDTH, height);
  EnableScrolling(false, false);
}

TableRowHeadersControl::~TableRowHeadersControl() {
  if (_caption_background_brush) {
    delete _caption_background_brush;
    _caption_background_brush = NULL;
  }

  if (_caption_grid_pen) {
    delete _caption_grid_pen;
    _caption_grid_pen = NULL;
  }
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
  dc->SetPen(*_caption_grid_pen);
  dc->SetBrush(*_caption_background_brush);

  // TODO for now we are drawing all available columns and rows
  // this can possibly be optimized

  // TODO: Move to other control
  // // Columns
  // c = 0;
  // x = ROW_HEADER_WIDTH; // Use row header width as offset
  //                       // for columns to leave some space
  //                       // from left
  // for (auto coldef : sheet->column_definitions) {
  //   // if (x > width) {
  //   //   break;
  //   // }

  //   auto name = coldef->caption;

  //   // TODO Move to tools
  //   if (name.empty()) {
  //     std::stringstream ss;
  //     ss << c;
  //     name = ss.str();
  //   }

  //   wxRect rect(x, 2, coldef->width, COLUMN_HEADER_HEIGHT);
  //   dc->DrawRectangle(rect);
  //   DrawTextInCenter(dc, name, rect);

  //   x += coldef->width;

  //   c++;
  // }

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

// TODO: Remove duplicated code
Location TableRowHeadersControl::GetScrollPosition() const {
  return Location(GetScrollPos(wxHORIZONTAL), GetScrollPos(wxVERTICAL));
}

// TODO: Remove duplicated code
void TableRowHeadersControl::DrawTextInCenter(wxDC *dc, const wxString &s,
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

wxSize TableRowHeadersControl::DoGetBestSize() const {
  int height = _document->get_current_sheet_height();

  // FIXME: Max header width should be calculated and returned by document
  // Remove fixed value
  return wxSize(ROW_HEADER_WIDTH, height);
}
