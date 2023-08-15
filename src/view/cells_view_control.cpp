#include "cells_view_control.h"
#include <cstddef>
#include <iostream>
#include <sstream>
#include <tuple>
#include <wx/clipbrd.h>
#include <wx/dcclient.h>

const int SCROLL_UNIT = 10;

CellsViewControl::CellsViewControl(TableWorkbookDocument *document,
                                   EventSink *event_sink, wxWindow *parent,
                                   wxWindowID id, const wxPoint &pos,
                                   const wxSize &size, long style)
    : wxScrolledWindow(parent, id, pos, size, style) {
  assert(event_sink);
  assert(document);

  _event_sink = event_sink;
  _document = document;

  Bind(wxEVT_CHAR_HOOK, &CellsViewControl::OnKeyPress, this);
  Bind(wxEVT_LEFT_DOWN, &CellsViewControl::OnLeftDown, this);

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

void CellsViewControl::OnDraw(wxDC &dc) {
  TableSheetPtr sheet = _document->current_sheet();
  if (!sheet) {
    // TODO Clear?
    return;
  }

  DrawTable(&dc, sheet);
}

void CellsViewControl::ScrollWindow(int dx, int dy, const wxRect *rect) {
  wxScrolledWindow::ScrollWindow(dx, dy, rect);
  wxPrintf("SCROLL EVENT: %d/%d\n", dx, dy);
  // m_colLabels->ScrollWindow(dx, 0, rect);
  // m_rowLabels->ScrollWindow(0, dy, rect);
  // TODO: Scroll row control and col control
  _event_sink->send_event(CELL_VIEW_SCROLL_EVENT, std::make_tuple(dx, dy, rect));
}

void CellsViewControl::OnCopy() {
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

void CellsViewControl::OnPaste() {
  if (wxTheClipboard->Open()) {
    if (wxTheClipboard->IsSupported(wxDF_TEXT)) {
      wxTextDataObject data;
      wxTheClipboard->GetData(data);
      // wxMessageBox(data.GetText());
      std::string content(data.GetText());
      _document->update_content_current_cell(content);
    }
    wxTheClipboard->Close();
  }
}

void CellsViewControl::OnCut() {
  OnCopy();

  _document->clear_current_cell();
}

void CellsViewControl::DrawTable(wxDC *dc, TableSheetPtr sheet) {
  Location scrollPos = GetScrollPosition();
  int width;
  int height;
  GetSize(&width, &height);

  // TODO With the above information we could calculate the viewport
  // contents and only draw these to avoid flickering?
  DrawCells(dc, scrollPos, width, height, sheet);
}

void CellsViewControl::RefreshScrollbars() {
  int width = _document->get_current_sheet_width() / SCROLL_UNIT;
  int height = _document->get_current_sheet_height() / SCROLL_UNIT;

  SetScrollbars(SCROLL_UNIT, SCROLL_UNIT, width, height);
}

Location CellsViewControl::GetScrollPosition() const {
  return Location(GetScrollPos(wxHORIZONTAL), GetScrollPos(wxVERTICAL));
}

void CellsViewControl::DrawCells(wxDC *dc, const Location &WXUNUSED(scrollPos),
                                 int WXUNUSED(width), int WXUNUSED(height),
                                 TableSheetPtr sheet) {
  // wxRect scrollArea = GetCurrentScrollArea();

  // TODO Only draw visible ones

  for (size_t r = 0; r < sheet->row_count(); r++) {
    for (size_t c = 0; c < sheet->col_count(); c++) {
      auto cell = sheet->get_cell(r, c);
      if (cell) {
        wxRect cellRect = GetCellRectByLocation(Location(c, r));
        // if (!scrollArea.Contains(cellRect)) {
        //   break;
        // }

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
  if (!current_cell_rect
           .IsEmpty() /*&& scrollArea.Contains(current_cell_rect)*/) {
    dc->SetPen(*_current_cell_pen);
    dc->SetBrush(*_window_brush);
    dc->DrawRectangle(current_cell_rect);

    auto cell = _document->get_current_cell();
    if (cell) {
      DrawTextInCenter(dc, cell->visible_content(), current_cell_rect);
    }
  }
}

CellsViewControl::~CellsViewControl() {
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

  if (_caption_background_brush) {
    delete _caption_background_brush;
    _caption_background_brush = NULL;
  }
}

wxRect CellsViewControl::GetCellRectByLocation(const Location &cell) {
  int x = 1;
  int y = 2;
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

wxRect CellsViewControl::GetCurrentScrollArea() const {
  wxPoint viewStart = GetViewStart();
  wxSize clientSize = GetClientSize();

  viewStart.x *= SCROLL_UNIT;
  viewStart.y *= SCROLL_UNIT;

  // Calculate the view rect
  return wxRect(viewStart, clientSize);
}

void CellsViewControl::OnKeyPress(wxKeyEvent &event) {
  bool cell_selection_moved = false;

  // Handle the keypress event here
  int keyCode = event.GetKeyCode();
  bool control = event.RawControlDown();

  // Example: Print the keycode to the console
  // wxPrintf("Key pressed: %d\n", keyCode);

  bool handled = true;

  switch (keyCode) {
  case WXK_UP:
    _document->move_cursor_up();
    cell_selection_moved = true;
    break;
  case WXK_DOWN:
    _document->move_cursor_down();
    cell_selection_moved = true;
    break;
  case WXK_LEFT:
    _document->move_cursor_left();
    cell_selection_moved = true;
    break;
  case WXK_RIGHT:
    _document->move_cursor_right();
    cell_selection_moved = true;
    break;
  case WXK_PAGEUP:
    _document->move_cursor_page_up();
    cell_selection_moved = true;
    break;
  case WXK_PAGEDOWN:
    _document->move_cursor_page_down();
    cell_selection_moved = true;
    break;
  case WXK_DELETE:
    _document->clear_current_cell();
    break;
  case WXK_F3: {
    int x, y;
    GetViewStart(&x, &y);
    wxPrintf("TEST view start: %d, %d\n", x, y);

    wxRect scrollArea = GetCurrentScrollArea();

    wxPrintf("TEST! Scroll Area: %d, %d, %d, %d, (right: %d, bottom: %d)\n",
             scrollArea.GetLeft(), scrollArea.GetTop(), scrollArea.GetWidth(),
             scrollArea.GetHeight(), scrollArea.GetRight(),
             scrollArea.GetBottom());

    break;
  }
  case WXK_F4: {
    // TEST
    Location loc(2, 40);
    _document->select_cell(loc);

    // wxRect rect = GetCellRectByLocation(loc);
    // wxPrintf("TEST! Cell Rect: %d, %d, %d, %d, (right: %d, bottom: %d)\n",
    //          rect.GetLeft(), rect.GetTop(), rect.GetWidth(),
    //          rect.GetHeight(), rect.GetRight(), rect.GetBottom());

    // Scroll(rect.GetLeft() / SCROLL_UNIT, rect.GetTop() / SCROLL_UNIT);

    ScrollToCell(loc, TOP);

    // TEST
    break;
  }
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

  if (cell_selection_moved) {
    ScrollToCurrentCell();
  }

  if (handled) {
    Refresh();
  } else {
    event.Skip(); // Allow further event handling
  }
}

void CellsViewControl::ScrollToCell(const Location &cell,
                                    TableCellOrientation orientation) {
  wxRect scrollArea = GetCurrentScrollArea();
  wxRect rect = GetCellRectByLocation(cell);
  // wxPrintf("TEST! Cell Rect: %d, %d, %d, %d, (right: %d, bottom: %d)\n",
  // rect.GetLeft(),
  //          rect.GetTop(), rect.GetWidth(), rect.GetHeight(),
  //          rect.GetRight(), rect.GetBottom());

  std::pair<TableRowDefinitionPtr, TableColumnDefinitionPtr> defs =
      _document->get_definitions_for_location(cell);

  int x, y;

  switch (orientation) {
  case TOP:
    x = scrollArea.GetLeft();
    y = rect.GetTop();
    break;
  case LEFT:
    x = rect.GetLeft() - defs.second->width;
    y = scrollArea.GetTop();
    break;
  case BOTTOM:
    x = scrollArea.GetLeft();
    y = rect.GetBottom() - scrollArea.GetHeight();
    break;
  case RIGHT:
    x = rect.GetRight() - scrollArea.GetWidth();
    y = scrollArea.GetTop();
    break;
  default:
    throw std::runtime_error("Unknown orientation");
  }

  x /= SCROLL_UNIT;
  y /= SCROLL_UNIT;

  Scroll(x, y);
}

void CellsViewControl::ScrollToCurrentCell() {
  wxRect scrollArea = GetCurrentScrollArea();
  TableCellPtr cell = _document->get_current_cell();
  wxRect cell_rect = GetCellRectByLocation(cell->location());

  // int scrollX, scrollY;
  // GetScrollPixelsPerUnit(&scrollX, &scrollY);

  // wxPrintf("Scroll Area: %d, %d, %d, %d, %d, %d\n", scrollArea.GetLeft(),
  //          scrollArea.GetTop(), scrollArea.GetWidth(),
  //          scrollArea.GetHeight(), scrollArea.GetRight(),
  //          scrollArea.GetBottom());
  // wxPrintf("  -> Cell Rect: %d, %d, %d, %d, %d, %d\n", cell_rect.GetLeft(),
  //          cell_rect.GetTop(), cell_rect.GetWidth(), cell_rect.GetHeight(),
  //          cell_rect.GetRight(), cell_rect.GetBottom());

  if (scrollArea.Contains(cell_rect)) {
    return;
  }

  if (cell_rect.GetLeft() < scrollArea.GetLeft()) {
    // Cell is left from scroll area
    ScrollToCell(cell->location(), LEFT);
  }

  if (cell_rect.GetTop() < scrollArea.GetTop()) {
    // Cell is above scroll area
    ScrollToCell(cell->location(), TOP);
  }

  if (cell_rect.GetBottom() > scrollArea.GetBottom()) {
    // Cell is below scroll area
    ScrollToCell(cell->location(), BOTTOM);
  }

  if (cell_rect.GetRight() > scrollArea.GetRight()) {
    // Cell is right from scroll area
    ScrollToCell(cell->location(), RIGHT);
  }
}

void CellsViewControl::OnCellUpdate(const Location &location) {
  wxRect rect = GetCurrentScrollArea();

  if (rect.Contains(location.x(), location.y())) {
    Refresh();
  }
}

void CellsViewControl::DrawTextInCenter(wxDC *dc, const wxString &s,
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

void CellsViewControl::OnLeftDown(wxMouseEvent &event) {
  wxPoint clickPosition = event.GetPosition();
  wxPrintf("Click Position: %d, %d\n", clickPosition.x, clickPosition.y);

  Location scrollPos = GetScrollPosition();
  wxPrintf("scroll pos: %ld,%ld\n", scrollPos.x(), scrollPos.y());

  int scrollPixelPerUnitX, scrollPixelPerUnitY;
  GetScrollPixelsPerUnit(&scrollPixelPerUnitX, &scrollPixelPerUnitY);
  wxPrintf("pixels per unit: %d,%d\n", scrollPixelPerUnitX,
           scrollPixelPerUnitY);

  long scrollPosX, scrollPosY;
  scrollPosX = scrollPos.x() * scrollPixelPerUnitX;
  scrollPosY = scrollPos.y() * scrollPixelPerUnitY;

  clickPosition.x += scrollPosX;
  clickPosition.y += scrollPosY;

  // Calculate the clicked coordinates accounting for the scroll position
  wxPrintf("Logical / calculated Click Position: %d, %d\n", clickPosition.x,
           clickPosition.y);

  Location cell = GetTableCellByClickPosition(clickPosition);

  // wxPrintf("Selected cell: %ld, %ld\n", cell.x(), cell.y());

  _document->select_cell(cell);

  event.Skip();
}

Location
CellsViewControl::GetTableCellByClickPosition(const wxPoint &pos) const {
  return _document->get_cell_by_pos(Location(pos.x, pos.y));
}
