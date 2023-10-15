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

#include "cells_view_control.h"
#include "kalkulator_system_colors.h"
#include <iostream>
#include <tuple>
#include <wx/clipbrd.h>
#include <wx/dcclient.h>

const int SCROLL_UNIT = 10;

CellsViewControl::CellsViewControl(KalkulatorSystemColorsPtr sys_colors,
                                   TableWorkbookDocumentPtr document,
                                   EventSink *event_sink, wxWindow *parent,
                                   wxWindowID id, const wxPoint &pos,
                                   const wxSize &size, long style)
    :
    TableSheetView(std::move(document),
                   event_sink,
                   parent,
                   id,
                   pos,
                   size,
                   style),
    _sys_colors(std::move(sys_colors)) {

  Bind(wxEVT_CHAR_HOOK, &CellsViewControl::OnKeyPress, this);
  Bind(wxEVT_LEFT_DOWN, &CellsViewControl::OnLeftDown, this);

  RefreshScrollbars();
}

void CellsViewControl::OnDraw(wxDC &dc) {
  DrawTable(&dc, _document->current_sheet());
}

void CellsViewControl::ScrollWindow(int dx, int dy, const wxRect *rect) {
  wxScrolledWindow::ScrollWindow(dx, dy, rect);
  _event_sink->send_event(CELL_VIEW_SCROLL_EVENT, GetScrollPosition());
}

void CellsViewControl::CopyString(const std::string &content) {
  if (wxTheClipboard->Open()) {
    wxString textToCopy = content.c_str();
    wxTheClipboard->SetData(new wxTextDataObject(textToCopy));
    wxTheClipboard->Close();
  }
}

void CellsViewControl::OnCopy() {
  TableCellPtr cell = _document->get_current_cell();
  if (cell) {
    std::string content = cell->visible_content();
    CopyString(content);
  }
}

void CellsViewControl::OnPaste() {
  if (wxTheClipboard->Open()) {
    if (wxTheClipboard->IsSupported(wxDF_TEXT)) {
      wxTextDataObject data;
      wxTheClipboard->GetData(data);
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

void CellsViewControl::DrawTable(wxDC *dc, const TableSheetPtr &sheet) {
  Location scrollPos = GetScrollPosition();
  int width;
  int height;
  GetSize(&width, &height);

  DrawCells(dc, scrollPos, width, height, sheet);
}

void CellsViewControl::RefreshScrollbars() {
  int width = _document->get_current_sheet_width() / SCROLL_UNIT;
  int height = _document->get_current_sheet_height() / SCROLL_UNIT;

  SetScrollbars(SCROLL_UNIT, SCROLL_UNIT, width, height);
}

void CellsViewControl::DrawCells(wxDC *dc, const Location &WXUNUSED(scrollPos),
                                 int WXUNUSED(width), int WXUNUSED(height),
                                 const TableSheetPtr &sheet) {
  wxRect scrollArea = GetCurrentScrollArea();
  wxRect current_cell_rect =
      GetCellRectByLocation(sheet->selection().primary()); // TODO Selection

  auto selected_cell_rects = collect_selected_cell_rects(sheet->selection());

  for (size_t r = 0; r < sheet->row_count(); r++) {

    if (_document->get_row_top_position(r)
        > static_cast<size_t>(scrollArea.GetBottom())) {
      break;
    }

    for (size_t c = 0; c < sheet->col_count(); c++) {
      if (_document->get_col_left_position(c)
          > static_cast<size_t>(scrollArea.GetRight())) {
        break;
      }

      wxColour oldForegroundColor;
      bool foreground_color_reset_required = false;

      wxFont oldFont;
      bool font_reset_required = false;

      auto cell = sheet->get_cell(r, c);
      if (cell) {
        wxRect cellRect = GetCellRectByLocation(Location(c, r));
        if (!scrollArea.Contains(cellRect)) {
          continue;
        }

        if (cellRect == current_cell_rect) {
          dc->SetPen(*_sys_colors->primary_cell_pen);
          dc->SetBrush(*_sys_colors->window_brush);
          dc->DrawRectangle(current_cell_rect);
        } else if (selected_cell_rects.find(cellRect)
            != selected_cell_rects.end()) {
          dc->SetPen(*_sys_colors->cell_selection_pen);
          dc->SetBrush(*_sys_colors->cell_selection_brush);
          dc->DrawRectangle(cellRect);
        }

        if (cell->has_format()) {
          const auto &format = cell->format();

          if (format.background_color) {
            wxColour color = fromTableCellColor(*(format.background_color));

            wxBrush oldBrush = dc->GetBrush();
            wxBrush brush(color);
            dc->SetBrush(brush);

            // Use brush with that color and fill rect of cell
            dc->DrawRectangle(cellRect);

            dc->SetBrush(oldBrush);
          }

          if (format.foreground_color) {
            wxColour color = fromTableCellColor(*(format.foreground_color));

            oldForegroundColor = dc->GetTextForeground();
            foreground_color_reset_required = true;

            dc->SetTextForeground(color);
          }

          wxFont font = dc->GetFont();
          oldFont = font;
          if (format.bold) {
            font.MakeBold();
          }

          if (format.italic) {
            font.MakeItalic();
          }

          if (format.underlined) {
            font.MakeUnderlined();
          }

          if (format.font_size) {
            font.SetPointSize(*format.font_size);
          }

          if (format.font_name) {
            font.SetFaceName(*format.font_name);
          }

          dc->SetFont(font);
          font_reset_required = true;
        }

        DrawTextInCenter(dc, cell->visible_content(), cellRect);

        if (foreground_color_reset_required) {
          dc->SetTextForeground(oldForegroundColor);
        }

        if (font_reset_required) {
          dc->SetFont(oldFont);
        }
      }
    }
  }
}

wxRect CellsViewControl::GetCellRectByLocation(const Location &cell) const {
  int x = 1;
  int y = 2;
  int n;

  wxRect result;

  TableColumnDefinitionPtr cell_coldef;
  TableRowDefinitionPtr cell_rowdef;

  TableSheetPtr sheet = _document->current_sheet();

  n = 0;
  for (const auto &coldef : sheet->column_definitions()) {
    if (n == cell.x()) {
      cell_coldef = coldef;
      break;
    }

    x += coldef->width;

    n++;
  }

  n = 0;
  for (const auto &rowdef : sheet->row_definitions()) {
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
  return {viewStart, clientSize};
}

void CellsViewControl::OnKeyPress(wxKeyEvent &event) {
  bool cell_selection_moved = false;

  // Handle the keypress event here
  int keyCode = event.GetKeyCode();
  bool control = event.RawControlDown();
  bool shift = event.ShiftDown();

  bool handled = true;

  switch (keyCode) {
  case WXK_UP:
    _document->move_cursor_up();
    cell_selection_moved = true;
    break;
  case WXK_DOWN:
    cell_selection_moved = _document->move_cursor_down();
    break;
  case WXK_LEFT:
    cell_selection_moved = _document->move_cursor_left();
    break;
  case WXK_RIGHT:
    cell_selection_moved = _document->move_cursor_right();
    break;
  case WXK_PAGEUP:
    cell_selection_moved = _document->move_cursor_page_up();
    break;
  case WXK_PAGEDOWN:
    cell_selection_moved = _document->move_cursor_page_down();
    break;
  case WXK_DELETE:
    _document->clear_current_cell();
    break;
  case 'Z':
    if (control) {
      _document->undo();
    }
    break;
  case 'Y':
    if (control) {
      _document->redo();
    }
    break;

  case WXK_F2: {
    _event_sink->send_event(EDIT_CELL, {});
    break;
  }

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
    // TEST START
    Location loc(50, 40);
    _document->select_cell(loc);

    ScrollToCell(loc, CELL_WINDOW_LOCATION_TOP);
    // TEST END

    break;
  }
  case 'C':
    if (control) {
      if (shift) {
        OnCopyFormula();
      } else {
        OnCopy();
      }
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
    ScrollToCurrentCell(CELL_WINDOW_LOCATION_UNSPECIFIED);
  }

  if (handled) {
    Refresh();
  } else {
    event.Skip();
  }
}

void CellsViewControl::ScrollToCell(const Location &cell,
                                    CellWindowLocation cell_window_location) {
  wxRect scrollArea = GetCurrentScrollArea();
  wxRect rect = GetCellRectByLocation(cell);

  std::pair<TableRowDefinitionPtr, TableColumnDefinitionPtr> defs =
      _document->get_definitions_for_location(cell);

  int x, y;

  switch (cell_window_location) {
  case CELL_WINDOW_LOCATION_TOP:
    x = scrollArea.GetLeft();
    y = rect.GetTop();
    break;
  case CELL_WINDOW_LOCATION_LEFT:
    x = rect.GetLeft() - defs.second->width;
    y = scrollArea.GetTop();
    break;
  case CELL_WINDOW_LOCATION_BOTTOM:
    x = scrollArea.GetLeft();
    y = rect.GetBottom() - scrollArea.GetHeight() + 10;
    break;
  case CELL_WINDOW_LOCATION_RIGHT:
    x = rect.GetRight() - scrollArea.GetWidth()
        + 10;
    y = scrollArea.GetTop();
    break;
  case CELL_WINDOW_LOCATION_CENTER:
    x = std::max(0,
                 rect.GetLeft()
                     + (rect.GetWidth() / 2)
                     - (scrollArea.GetWidth()
                         / 2));
    y = std::max(0,
                 rect.GetTop() + (rect.GetHeight() / 2)
                     - (scrollArea.GetHeight() / 2));
    break;
  default:
    throw std::runtime_error("Unhandled cell window location");
  }

  x /= SCROLL_UNIT;
  y /= SCROLL_UNIT;

  Scroll(x, y);
}

void CellsViewControl::ScrollToCurrentCell(CellWindowLocation location) {
  wxRect scrollArea = GetCurrentScrollArea();
  TableCellPtr cell = _document->get_current_cell();
  wxRect cell_rect = GetCellRectByLocation(cell->location());

  if (scrollArea.Contains(cell_rect)) {
    return;
  }

  CellWindowLocation actual_location = location;

  if (actual_location == CELL_WINDOW_LOCATION_UNSPECIFIED) {
    actual_location = GetCellLocation(cell_rect, scrollArea);
  }

  ScrollToCell(cell->location(), actual_location);
}

CellWindowLocation CellsViewControl::GetCellLocation(const wxRect &cell_rect,
                                                     const wxRect &scrollArea) const {
  if (cell_rect.GetLeft() < scrollArea.GetLeft()) {
    // Cell is left from scroll area
    return CELL_WINDOW_LOCATION_LEFT;
  }

  if (cell_rect.GetTop() < scrollArea.GetTop()) {
    // Cell is above scroll area
    return CELL_WINDOW_LOCATION_TOP;
  }

  if (cell_rect.GetBottom() > scrollArea.GetBottom()) {
    // Cell is below scroll area
    return CELL_WINDOW_LOCATION_BOTTOM;
  }

  if (cell_rect.GetRight() > scrollArea.GetRight()) {
    // Cell is right from scroll area
    return CELL_WINDOW_LOCATION_RIGHT;
  }

  return CELL_WINDOW_LOCATION_CENTER;
}

void CellsViewControl::OnCellUpdate(const Location &location) {
  wxRect rect = GetCurrentScrollArea();

  if (rect.Contains(location.x(), location.y())) {
    Refresh();
  }
}

void CellsViewControl::OnLeftDown(wxMouseEvent &event) {
  bool control = event.RawControlDown();

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
  Location cell = GetTableCellByClickPosition(clickPosition);

  if (control) {
    _document->selection_add_cell(cell);
  } else {
    _document->select_cell(cell);
  }

  event.Skip();
}

Location
CellsViewControl::GetTableCellByClickPosition(const wxPoint &pos) const {
  return _document->get_cell_by_pos(Location(pos.x, pos.y));
}

void CellsViewControl::OnCopyFormula() {
  TableCellPtr cell = _document->get_current_cell();
  if (cell) {
    std::string content = cell->get_formula_content();
    CopyString(content);
  }
}

wxColour CellsViewControl::fromTableCellColor(const TableCellColor &color) {
  return {color.r, color.g, color.b};
}

void CellsViewControl::Initialize() {
  SetBackgroundColour(_sys_colors->window_color);
  SetForegroundColour(_sys_colors->window_text_color);
}

WxRectSet CellsViewControl::collect_selected_cell_rects(
    const TableCellSelection &selection) const {
  WxRectSet result;

  for (const auto &cell : selection.additional_cells()) {
    auto rect = GetCellRectByLocation(cell);
    result.insert(rect);
  }

  return result;
}
