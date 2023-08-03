#include "table_control.h"
#include <cstddef>
#include <iostream>
#include <sstream>
#include <wx/clipbrd.h>
#include <wx/dcclient.h>

const int ROW_HEADER_WIDTH = 50;
const int COLUMN_HEADER_HEIGHT = 30;

TableControl::TableControl(TableWorkbookDocument *document,
                           EventSink *event_sink, wxWindow *parent,
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
  Bind(wxEVT_LEFT_DOWN, &TableControl::OnLeftDown, this);

  RefreshScrollbars();

  _window_color = wxSystemSettingsNative::GetColour(wxSYS_COLOUR_WINDOW);
  _window_text_color =
      wxSystemSettingsNative::GetColour(wxSYS_COLOUR_WINDOWTEXT);
  _button_face_color = wxSystemSettingsNative::GetColour(wxSYS_COLOUR_BTNFACE);
  _button_text_color = wxSystemSettingsNative::GetColour(wxSYS_COLOUR_BTNTEXT);
  _active_border_color =
      wxSystemSettingsNative::GetColour(wxSYS_COLOUR_HIGHLIGHT);

  SetBackgroundColour(_window_color);
  SetForegroundColour(_window_text_color);

  _window_brush = new wxBrush(_window_color);

  _caption_background_brush = new wxBrush(_button_face_color);
  //_caption_grid_pen = new wxPen(wxColour(145, 145, 145));
  _caption_grid_pen = new wxPen(_button_text_color);
  _grid_pen = new wxPen(wxColour(100, 100, 100));
  //_current_cell_pen = new wxPen(wxColour(47, 65, 163), 2);
  _current_cell_pen = new wxPen(_active_border_color);
}

void TableControl::OnDraw(wxDC &dc) {
  TableSheetPtr sheet = _document->current_sheet();
  if (!sheet) {
    // TODO Clear?
    return;
  }

  DrawTable(&dc, sheet);
}

void TableControl::OnCopy() {
  TableCellPtr cell = _document->get_current_cell();
  if (cell) {
    std::string content = cell->get_formula_content();
    if (wxTheClipboard->Open()) {
      wxString textToCopy = content.c_str();
      wxTheClipboard->SetData(new wxTextDataObject(textToCopy));
      wxTheClipboard->Close();
    }
  }
}

void TableControl::OnPaste() {
  if (wxTheClipboard->Open()) {
    if (wxTheClipboard->IsSupported(wxDF_TEXT)) {
      wxTextDataObject data;
      wxTheClipboard->GetData(data);
      //wxMessageBox(data.GetText());
      std::string content(data.GetText());
      _document->update_content_current_cell(content);
    }
    wxTheClipboard->Close();
  }
}

void TableControl::OnCut() {
  OnCopy();

  _document->clear_current_cell();
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

void TableControl::OnScroll(wxScrollWinEvent &WXUNUSED(scrollEvent)) {
  int x = GetScrollPos(wxHORIZONTAL);
  int y = GetScrollPos(wxVERTICAL);

  // Not needed here, but can be useful later (e.g. search for specific cell)
  // Scroll(x, y);

  // std::cout << "Scroll: x: " << x << ", y: " << y << std::endl;

  // scrollEvent.Skip();
}

void TableControl::RefreshScrollbars() {
  int width = _document->get_current_sheet_width();
  int height = _document->get_current_sheet_height();

  SetScrollbars(1, 1, width, height, 0, 0);
}

Location TableControl::GetScrollPosition() const {
  return Location(GetScrollPos(wxHORIZONTAL), GetScrollPos(wxVERTICAL));
}

void TableControl::DrawHeaders(wxDC *dc, const Location &WXUNUSED(scrollPos),
                               int width, int height, TableSheetPtr sheet) {
  int x, y, c;

  // Set pen and brushes for headers of columns and rows
  dc->SetPen(*_caption_grid_pen);
  dc->SetBrush(*_caption_background_brush);

  // TODO for now we are drawing all available columns and rows
  // this can possibly be optimized

  // Columns
  c = 0;
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
  c = 0;
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

void TableControl::DrawCells(wxDC *dc, const Location &WXUNUSED(scrollPos),
                             int WXUNUSED(width), int WXUNUSED(height),
                             TableSheetPtr sheet) {
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
    dc->SetBrush(*_window_brush);
    dc->DrawRectangle(current_cell_rect);

    auto cell = _document->get_current_cell();
    if (cell) {
      DrawTextInCenter(dc, cell->visible_content(), current_cell_rect);
    }
  }
}

TableControl::~TableControl() {
  if (_window_brush) {
    delete _window_brush;
    _window_brush = NULL;
  }

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
  bool control = event.RawControlDown();

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
  case WXK_PAGEUP:
    _document->move_cursor_page_up();
    break;
  case WXK_PAGEDOWN:
    _document->move_cursor_page_down();
    break;
  case 'C':
    if (control) {
      OnCopy();
    }
    break;
  case 'V':
    if (control) {
      OnPaste();
    }
    break;
  case 'X':
    if (control) {
      OnCut();
    }
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

void TableControl::OnCellUpdate(const Location &location) {
  wxRect rect = GetCurrentScrollArea();

  if (rect.Contains(location.x(), location.y())) {
    Refresh();
  }
}

void TableControl::DrawTextInCenter(wxDC *dc, const wxString &s,
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

void TableControl::OnLeftDown(wxMouseEvent &event) {
  Location scrollPos = GetScrollPosition();
  wxPoint pos = event.GetPosition();
  wxPoint delta(pos.x + scrollPos.x() - ROW_HEADER_WIDTH,
                pos.y + scrollPos.y() - COLUMN_HEADER_HEIGHT);

  wxPrintf("Mouse Left Down! %d, %d\n", delta.x, delta.y);

  Location cell = GetTableCellByClickPosition(delta);

  wxPrintf("Selected cell: %ld, %ld\n", cell.x(), cell.y());

  _document->select_cell(cell);

  event.Skip();
}

Location TableControl::GetTableCellByClickPosition(const wxPoint &pos) const {
  return _document->get_cell_by_pos(Location(pos.x, pos.y));
}
