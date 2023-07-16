#include "table_control.h"
#include <cstddef>
#include <iostream>
#include <sstream>
#include <wx/dcclient.h>

const int ROW_HEADER_WIDTH = 50;
const int COLUMN_HEADER_HEIGHT = 30;

TableControl::TableControl(TableWorkbookDocument* document, EventSink *event_sink, wxWindow *parent,
                           wxWindowID id, const wxPoint &pos,
                           const wxSize &size, long style)
    : wxScrolledWindow(parent, id, pos, size, style) {
  assert(event_sink);
  assert(document);

  _event_sink = event_sink;
  _document = document;

  // Bind(wxEVT_PAINT, &TableControl::OnPaint, this);
  Bind(wxEVT_SCROLLWIN_THUMBTRACK, &TableControl::OnScroll, this);
  Bind(wxEVT_SCROLLWIN_THUMBRELEASE, &TableControl::OnScroll, this);
  Bind(wxEVT_SCROLLWIN_LINEUP, &TableControl::OnScroll, this);
  Bind(wxEVT_SCROLLWIN_LINEDOWN, &TableControl::OnScroll, this);
  Bind(wxEVT_CHAR_HOOK, &TableControl::OnKeyPress, this);

  SetBackgroundColour(wxColour(*wxWHITE));

  RefreshScrollbars();

  _caption_grid_pen = new wxPen(wxColour(145, 145, 145));
  _grid_pen = new wxPen(wxColour(100, 100, 100));
  _current_cell_pen = new wxPen(wxColour(47, 65, 163), 2);
}

void TableControl::OnDraw(wxDC &dc) {
  TableSheetPtr sheet = _document->current_sheet();
  if (!sheet) {
    // TODO Clear?
    return;
  }

  // wxClientDC dc(this);

  // DrawHeaders();
  DrawTable(&dc, sheet);
}

void TableControl::DrawTable(wxDC *dc, TableSheetPtr sheet) {
  Location scrollPos = GetScrollPosition();
  int width;
  int height;
  GetSize(&width, &height);

  // TODO With the above information we could calculate the viewport
  // contents and only draw these to avoid flickering?
  DrawHeaders(dc, scrollPos, width, height, sheet);
  DrawCells(dc, scrollPos, width, height, sheet);
}

void TableControl::OnScroll(wxScrollWinEvent &scrollEvent) {
  int x = GetScrollPos(wxHORIZONTAL);
  int y = GetScrollPos(wxVERTICAL);

  // Not needed here, but can be useful later (e.g. search for specific cell)
  // Scroll(x, y);

  // std::cout << "Scroll: x: " << x << ", y: " << y << std::endl;

  // scrollEvent.Skip();
}

void TableControl::RefreshScrollbars() {
  // TODO Align scroll area with actual content
  SetScrollbars(1, 1, 10000, 200000, 0, 0);
}

Location TableControl::GetScrollPosition() const {
  return Location(GetScrollPos(wxHORIZONTAL), GetScrollPos(wxVERTICAL));
}

void TableControl::DrawHeaders(wxDC *dc, const Location &scrollPos, int width,
                               int height, TableSheetPtr sheet) {
  int x, y, c;

  // Set pen and brushes for headers of columns and rows
  dc->SetPen(*_caption_grid_pen);
  dc->SetBrush(*wxLIGHT_GREY_BRUSH);

  // TODO for now we are drawing all available columns and rows
  // this can possibly be optimized

  // Columns
  c = 1;
  x = ROW_HEADER_WIDTH; // Use row header width as offset
                        // for columns to leave some space
                        // from left
  for (auto coldef : sheet->column_definitions) {
    if (x > width) {
      break;
    }

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

  // Rows
  c = 1;
  y = COLUMN_HEADER_HEIGHT + 2;
  for (auto rowdef : sheet->row_definitions) {
    if (y > height) {
      break;
    }

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

void TableControl::DrawCells(wxDC *dc, const Location &scrollPos, int width,
                             int height, TableSheetPtr sheet) {
  wxRect scrollArea = GetCurrentScrollArea();

  // TODO Only draw visible ones

  for (size_t r = 0; r < sheet->row_count(); r++) {
    for (size_t c = 0; c < sheet->col_count(); c++) {
      auto cell = sheet->get_cell(r, c);
      if (cell) {

        wxRect cellRect = GetCellRectByLocation(Location(c, r));
        if (!scrollArea.Contains(cellRect)) {
          break;
        }

        auto unwrapped_cell = *cell; // unwrap optional
        DrawTextInCenter(dc, unwrapped_cell->visible_content(), cellRect);
      }
    }
  }

  // Draw cursor(s)
  // TODO support multiple cursors?
  // for (auto &l : sheet->cursors) {
  //   TableRowDefinitionPtr rowdef;
  //   TableColumnDefinitionPtr coldef;

  //   std::tie(rowdef, coldef) = sheet->get_definitions_for_location(l);

  //   // TODO draw cursor
  // }

  // Current cell
  wxRect current_cell_rect = GetCellRectByLocation(sheet->current_cell);

  if (!current_cell_rect.IsEmpty() && scrollArea.Contains(current_cell_rect)) {
    dc->SetPen(*_current_cell_pen);
    dc->SetBrush(*wxWHITE_BRUSH);
    dc->DrawRectangle(current_cell_rect);
  }
}

TableControl::~TableControl() {
  if (_grid_pen) {
    delete _grid_pen;
    _grid_pen = NULL;
  }

  if (_caption_grid_pen) {
    delete _caption_grid_pen;
    _caption_grid_pen = NULL;
  }

  if (_current_cell_pen) {
    delete _current_cell_pen;
    _current_cell_pen = NULL;
  }
}

wxRect TableControl::GetCellRectByLocation(const Location &cell) {
  int x = ROW_HEADER_WIDTH + 1;
  int y = COLUMN_HEADER_HEIGHT + 2;
  int n;

  wxRect result;

  TableColumnDefinitionPtr cell_coldef;
  TableRowDefinitionPtr cell_rowdef;

  TableSheetPtr sheet = _document->current_sheet();

  n = 0;
  for (const auto &coldef : sheet->column_definitions) {
    if (n == cell.x()) {
      cell_coldef = coldef;
      break;
    }

    x += coldef->width;

    n++;
  }

  n = 0;
  for (const auto &rowdef : sheet->row_definitions) {
    if (n == cell.y()) {
      cell_rowdef = rowdef;
      break;
    }

    y += rowdef->height;

    n++;
  }

  assert(cell_rowdef);
  assert(cell_coldef);

  result.x = x;
  result.y = y;
  result.width = cell_coldef->width;
  result.height = cell_rowdef->height;

  return result;
}

wxRect TableControl::GetCurrentScrollArea() const {
  wxRect clientRect = GetClientRect();

  int scrollX, scrollY;
  GetViewStart(&scrollX, &scrollY);

  wxRect scrollArea;
  scrollArea.x = scrollX;
  scrollArea.y = scrollY;
  scrollArea.width = clientRect.width;
  scrollArea.height = clientRect.height;

  return scrollArea;
}

void TableControl::OnKeyPress(wxKeyEvent &event) {
  // Handle the keypress event here
  int keyCode = event.GetKeyCode();

  // Example: Print the keycode to the console
  wxPrintf("Key pressed: %d\n", keyCode);

  bool handled = true;

  switch (keyCode) {
  case WXK_UP:
    _document->move_cursor_up();
    break;
  case WXK_DOWN:
    _document->move_cursor_down();
    break;
  case WXK_LEFT:
    _document->move_cursor_left();
    break;
  case WXK_RIGHT:
    _document->move_cursor_right();
    break;
  default:
    handled = false;
    break;
  }

  if (handled) {
    Refresh();
  }

  event.Skip(); // Allow further event handling
}

void TableControl::OnCellUpdate(const Location& location) {
  wxRect rect = GetCurrentScrollArea();

  if (rect.Contains(location.x(), location.y())) {
    Refresh();
  }
}

void TableControl::DrawTextInCenter(wxDC* dc, const wxString& s, const wxRect& rect) {
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
