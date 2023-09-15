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


#ifndef CELLS_VIEW_CONTROL_INCLUDED
#define CELLS_VIEW_CONTROL_INCLUDED

#include "../model/table/table_cell_orientation.h"
#include "kalkulator_system_colors.h"
#include "table_sheet_view.h"

class CellsViewControl : public TableSheetView {
public:
  CellsViewControl(KalkulatorSystemColorsPtr sys_colors,
                   TableWorkbookDocumentPtr document, EventSink *event_sink,
                   wxWindow *parent, wxWindowID id = wxID_ANY,
                   const wxPoint &pos = wxDefaultPosition,
                   const wxSize &size = wxDefaultSize,
                   long style = wxTAB_TRAVERSAL);

  virtual ~CellsViewControl() = default;

  void OnDraw(wxDC &dc);
  void OnKeyPress(wxKeyEvent &event);
  void OnLeftDown(wxMouseEvent &event);
  void OnCopy();
  void OnCopyFormula();
  void OnPaste();
  void OnCut();

  void RefreshScrollbars();

  void DrawTable(wxDC *dc, TableSheetPtr sheet);
  void OnCellUpdate(const Location &location);

  virtual void ScrollWindow(int dx, int dy, const wxRect *rect);
  void ScrollToCurrentCell();

private:
  void DrawCells(wxDC *dc, const Location &scrollPos, int width, int height,
                 TableSheetPtr sheet);
  wxRect GetCellRectByLocation(const Location &cell);
  wxRect GetCurrentScrollArea() const;

  Location GetTableCellByClickPosition(const wxPoint &pos) const;
  void ScrollToCell(const Location &cell, TableCellOrientation orientation);

  void CopyString(const std::string &content);
  wxColour fromTableCellColor(const TableCellColor& color);

private:
  KalkulatorSystemColorsPtr _sys_colors;
};

#endif
