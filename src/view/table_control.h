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

#ifndef TABLECONTROL_INCLUDED
#define TABLECONTROL_INCLUDED

#include <wx/sizer.h>
#include <wx/wx.h>

#include "../model/event_sink.h"
#include "../model/table/table_workbook_document.h"
#include "cells_view_control.h"
#include "kalkulator_system_colors.h"
#include "location.h"
#include "table_column_headers_control.h"
#include "table_row_headers_control.h"

class TableControl : public wxWindow {
public:
  TableControl(KalkulatorSystemColorsPtr sys_colors,
               TableWorkbookDocumentPtr document, EventSink *event_sink,
               wxWindow *parent, wxWindowID id = wxID_ANY,
               const wxPoint &pos = wxDefaultPosition,
               const wxSize &size = wxDefaultSize,
               long style = wxTAB_TRAVERSAL);
  virtual ~TableControl();

  // Delete copy constructor and assignment operator
  TableControl(const TableControl &other) = delete;
  TableControl &operator=(const TableControl &other) = delete;

  void OnCellUpdate(const Location &location);
  void OnSetFocus(wxFocusEvent &event);
  void update_scroll_positions(const Location &scroll_pos);
  void ScrollToCurrentCell();

  wxFont GetCellViewFont() const { return _cells_view_control->GetFont(); }

protected:
  void Init();
  void CreateHeaderControls();
  void CreateCellsViewControl();

private:
  EventSink *_event_sink;
  TableWorkbookDocumentPtr _document;
  KalkulatorSystemColorsPtr _sys_colors;

  wxSizer *_top_sizer;
  wxSizer *_row_cell_view_sizer;
  CellsViewControl *_cells_view_control;
  TableRowHeadersControl *_row_headers_control;
  TableColumnHeadersControl *_column_headers_control;
};

#endif
