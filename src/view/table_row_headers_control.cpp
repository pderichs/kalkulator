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

#include "table_row_headers_control.h"
#include "kalkulator_system_colors.h"

//  TODO: Remove these as soon as possible!
const int ROW_HEADER_WIDTH = 50;

// NOTE Vertical scrolling is disabled by style
TableRowHeadersControl::TableRowHeadersControl(
    KalkulatorSystemColorsPtr sys_colors, TableWorkbookDocumentPtr document,
    EventSink *event_sink, wxWindow *parent, wxWindowID id, const wxPoint &pos,
    const wxSize &size, long style)
    : TableSheetView(document, event_sink, parent, id, pos, size,
                     style | ~wxVSCROLL),
      _sys_colors(sys_colors), _current_row(0) {
  int height = document->get_current_sheet_height();
  SetScrollRate(0, 10);
  SetVirtualSize(ROW_HEADER_WIDTH, height);

  Bind(wxEVT_SET_FOCUS, &TableRowHeadersControl::OnFocus, this);
}

void TableRowHeadersControl::Initialize() {
  EnableScrolling(false, false);
}

void TableRowHeadersControl::OnFocus(wxFocusEvent &WXUNUSED(event)) {
  _event_sink->send_event(HEADER_GOT_FOCUS, {});
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
                                         const TableSheetPtr& sheet) {
  std::ignore = width;
  std::ignore = height;
  std::ignore = scrollPos;

  int y;
  size_t c = 0;

  // Set pen and brushes for headers of columns and rows
  dc->SetPen(*_sys_colors->caption_grid_pen);

  // Rows
  y = 2;
  for (const auto& rowdef : sheet->row_definitions) {
    // if (y > height) {
    //   break;
    // }

    auto name = rowdef->caption;

    if (name.empty()) {
      std::stringstream ss;
      ss << c;
      name = ss.str();
    }

    if (c == _current_row) {
      dc->SetBrush(wxColour(*_sys_colors->current_cell_pen->GetColour()));
    } else {
      dc->SetBrush(*_sys_colors->caption_background_brush);
    }

    wxRect rect(0, y, ROW_HEADER_WIDTH, rowdef->height);
    dc->DrawRectangle(rect);
    DrawTextInCenter(dc, name, rect);

    y += rowdef->height;

    c++;
  }
}

wxSize TableRowHeadersControl::DoGetBestSize() const {
  return {ROW_HEADER_WIDTH, 100};
}

void TableRowHeadersControl::SetCurrentRow(size_t row) {
  _current_row = row;
}
