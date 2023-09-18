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


#ifndef TABLE_COLUMN_HEADERS_CONTROL_INCLUDED
#define TABLE_COLUMN_HEADERS_CONTROL_INCLUDED

#include "kalkulator_system_colors.h"
#include "table_sheet_view.h"

class TableColumnHeadersControl : public TableSheetView {
public:
  TableColumnHeadersControl(KalkulatorSystemColorsPtr sys_colors,
                            TableWorkbookDocumentPtr document,
                            EventSink *event_sink, wxWindow *parent,
                            wxWindowID id = wxID_ANY,
                            const wxPoint &pos = wxDefaultPosition,
                            const wxSize &size = wxDefaultSize,
                            long style = wxTAB_TRAVERSAL);
  virtual ~TableColumnHeadersControl() = default;

  void OnDraw(wxDC &dc);
  virtual wxSize DoGetBestSize() const;
  void OnFocus(wxFocusEvent& WXUNUSED(event));

private:
  void DrawHeaders(wxDC *dc, const Location &scrollPos, int width, int height,
                   const TableSheetPtr& sheet);

private:
  KalkulatorSystemColorsPtr _sys_colors;
};

#endif
