#include "table_control.h"
#include <iostream>
#include <wx/dcclient.h>

TableControl::TableControl(wxWindow *parent, wxWindowID id, const wxPoint &pos,
                           const wxSize &size, long style)
    : wxScrolledWindow(parent, id, pos, size, style) {
  // Bind(wxEVT_PAINT, &TableControl::OnPaint, this);
  Bind(wxEVT_SCROLLWIN_THUMBTRACK, &TableControl::OnScroll, this);
  Bind(wxEVT_SCROLLWIN_THUMBRELEASE, &TableControl::OnScroll, this);
  Bind(wxEVT_SCROLLWIN_LINEUP, &TableControl::OnScroll, this);
  Bind(wxEVT_SCROLLWIN_LINEDOWN, &TableControl::OnScroll, this);

  SetBackgroundColour(wxColour(*wxWHITE));

  RefreshScrollbars();
}

void TableControl::OnDraw(wxDC &dc) {
  if (!_sheet) {
    // TODO Clear?
    return;
  }

  // wxClientDC dc(this);

  // DrawHeaders();
  DrawGrid(&dc);
}

void TableControl::DrawGrid(wxDC *dc) {
  // TODO HBI

  dc->SetBrush(*wxBLUE);
  dc->SetPen(*wxRED);

  DrawHeaders(dc);
  DrawCells(dc);
}

void TableControl::OnScroll(wxScrollWinEvent &scrollEvent) {
  int x = GetScrollPos(wxHORIZONTAL);
  int y = GetScrollPos(wxVERTICAL);

  // Not needed here, but can be useful later (e.g. search for specific cell)
  // Scroll(x, y);

  std::cout << "Scroll: x: " << x << ", y: " << y << std::endl;

  scrollEvent.Skip();
}

void TableControl::RefreshScrollbars() {
  // TODO
  SetScrollbars(1, 1, 10000, 200000, 0, 0);
}

Location TableControl::GetScrollPosition() const {
  return Location(
    GetScrollPos(wxHORIZONTAL),
    GetScrollPos(wxVERTICAL));
}

void TableControl::DrawHeaders(wxDC* dc) {
  Location scrollPos = GetScrollPosition();

  int width;
  int height;

  GetSize(&width, &height);

  // TODO: Column definitions and sizes are missing atm - must be added first
}

void TableControl::DrawCells(wxDC* dc) {

}
