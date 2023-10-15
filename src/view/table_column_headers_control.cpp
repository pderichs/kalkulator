/**
 * Kalkulator - a simple and small spread sheet app.
 * Copyright (C) 2023  pderichs (derichs@posteo.de)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "table_column_headers_control.h"

const int ROW_HEADER_WIDTH = 50;
const int COLUMN_HEADER_HEIGHT = 30;

// NOTE Horizontal scroll bar is disabled by style
TableColumnHeadersControl::TableColumnHeadersControl(
    KalkulatorSystemColorsPtr sys_colors, TableWorkbookDocumentPtr document,
    EventSink *event_sink, wxWindow *parent, wxWindowID id, const wxPoint &pos,
    const wxSize &size, long style)
    : TableSheetView(document, event_sink, parent, id, pos, size,
                     style | ~wxHSCROLL),
      _sys_colors(sys_colors), _current_column(0) {
  int width = _document->get_current_sheet_width() + ROW_HEADER_WIDTH;
  SetScrollRate(10, 0);
  SetVirtualSize(width, COLUMN_HEADER_HEIGHT);

  Bind(wxEVT_SET_FOCUS, &TableColumnHeadersControl::OnFocus, this);
}

void TableColumnHeadersControl::Initialize() {
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
                                            const TableSheetPtr &sheet) {
  std::ignore = width;
  std::ignore = height;
  std::ignore = scrollPos;

  int x;
  size_t c = 0;

  // Set pen and brushes for headers of columns and rows
  dc->SetPen(*_sys_colors->caption_grid_pen);

  // TODO for now we are drawing all available columns and rows
  // this can possibly be optimized

  // Columns
  x = ROW_HEADER_WIDTH;
  for (const auto &coldef : sheet->column_definitions()) {
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

    if (c == _current_column) {
      dc->SetBrush(wxColour(*_sys_colors->primary_cell_pen->GetColour()));
    } else {
      dc->SetBrush(*_sys_colors->caption_background_brush);
    }

    wxRect rect(x, 2, coldef->width, COLUMN_HEADER_HEIGHT);
    dc->DrawRectangle(rect);
    DrawTextInCenter(dc, name, rect);

    x += coldef->width;

    c++;
  }
}

wxSize TableColumnHeadersControl::DoGetBestSize() const {
  return {100, COLUMN_HEADER_HEIGHT};
}

void TableColumnHeadersControl::OnFocus(wxFocusEvent &WXUNUSED(event)) {
  _event_sink->send_event(HEADER_GOT_FOCUS, {});
}

void TableColumnHeadersControl::SetCurrentCol(size_t col) {
  _current_column = col;
}
