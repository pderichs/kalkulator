#include "table_control.h"
#include <iostream>
#include <sstream>
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

  Location scrollPos = GetScrollPosition();
  int width;
  int height;
  GetSize(&width, &height);

  // TODO With the above information we could calculate the viewport
  // contents and only draw these to avoid flickering?
  DrawHeaders(dc, scrollPos, width, height);
  DrawCells(dc, scrollPos, width, height);
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
  return Location(GetScrollPos(wxHORIZONTAL), GetScrollPos(wxVERTICAL));
}

void TableControl::DrawHeaders(wxDC *dc, const Location &scrollPos, int width,
                               int height) {
  int x, y, c;

  dc->SetPen(*wxBLACK);
  dc->SetBrush(*wxLIGHT_GREY_BRUSH);

  // FIXME for now we are drawing all available columns and rows

  c = 1;
  x = 0;
  for (auto coldef : _sheet->column_definitions) {
    auto name = coldef->caption;

    if (name.empty()) {
      std::stringstream ss;
      ss << c;
      name = ss.str();
    }

    dc->DrawRectangle(x, 2, x + coldef->width, 20);
    dc->DrawText(name, x + 2, 4);

    x += coldef->width;

    c++;
  }

  // for (auto rowdef : _sheet->row_definitions) {
  // }
}

void TableControl::DrawCells(wxDC *dc, const Location &scrollPos, int width,
                             int height) {}
