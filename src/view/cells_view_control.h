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

#include "kalkulator_system_colors.h"
#include "table_sheet_view.h"

/**
 * Less than operator for wxRect usage in std::set
 */
const auto wxrect_less_fct = [](const wxRect &r1, const wxRect &r2) -> bool {
  return r1.GetLeft() < r2.GetLeft()
      || ((r1.GetLeft() == r2.GetLeft()) && (r1.GetTop() < r2.GetTop()));
};

/**
 * Set which contains wxRect
 */
typedef std::set<wxRect, decltype(wxrect_less_fct)> WxRectSet;

enum CellWindowLocation {
  CELL_WINDOW_LOCATION_UNSPECIFIED,
  CELL_WINDOW_LOCATION_LEFT,
  CELL_WINDOW_LOCATION_TOP,
  CELL_WINDOW_LOCATION_RIGHT,
  CELL_WINDOW_LOCATION_BOTTOM,
  CELL_WINDOW_LOCATION_CENTER
};

/**
 * This is a custom UI control providing the cell view of a sheet.
 */
class CellsViewControl : public TableSheetView {
public:
  CellsViewControl(KalkulatorSystemColorsPtr sys_colors,
                   TableWorkbookDocumentPtr document, EventSink *event_sink,
                   wxWindow *parent, wxWindowID id = wxID_ANY,
                   const wxPoint &pos = wxDefaultPosition,
                   const wxSize &size = wxDefaultSize,
                   long style = wxTAB_TRAVERSAL);

  ~CellsViewControl() override = default;

  void Initialize();

  void OnDraw(wxDC &dc) override;
  void OnKeyPress(wxKeyEvent &event);
  void OnLeftDown(wxMouseEvent &event);
  void OnCopy();
  void OnCopyFormula();
  void OnPaste();
  void OnCut();

  void RefreshScrollbars();

  void DrawTable(wxDC *dc, const TableSheetPtr &sheet);
  void OnCellUpdate(const Location &location);

  void ScrollWindow(int dx, int dy, const wxRect *rect) override;
  void ScrollToCurrentCell(CellWindowLocation location);

private:
  void DrawCells(wxDC *dc, const Location &scrollPos, int width, int height,
                 const TableSheetPtr &sheet);
  wxRect GetCellRectByLocation(const Location &cell) const;
  wxRect GetCurrentScrollArea() const;

  Location GetTableCellByClickPosition(const wxPoint &pos) const;
  void ScrollToCell(const Location &cell,
                    CellWindowLocation cell_window_location);

  void CopyString(const std::string &content);
  static wxColour fromTableCellColor(const TableCellColor &color);

  CellWindowLocation GetCellLocation(const wxRect &cell_rect,
                                     const wxRect &scrollArea) const;

private:
  WxRectSet collect_selected_cell_rects(const TableCellSelection &selection) const;

private:
  KalkulatorSystemColorsPtr _sys_colors;
};

#endif
