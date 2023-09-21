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

#include "table_control.h"
#include "table_column_headers_control.h"
#include "table_row_headers_control.h"
#include <iostream>
#include <tuple>
#include <wx/clipbrd.h>

TableControl::TableControl(KalkulatorSystemColorsPtr sys_colors,
                           TableWorkbookDocumentPtr document,
                           EventSink *event_sink, wxWindow *parent,
                           wxWindowID id, const wxPoint &pos,
                           const wxSize &size, long style)
    : wxWindow(parent, id, pos, size, style), _event_sink(event_sink),
      _document(document), _sys_colors(sys_colors), _top_sizer(nullptr),
      _row_cell_view_sizer(nullptr), _cells_view_control(nullptr),
      _row_headers_control(nullptr), _column_headers_control(nullptr) {
  Init();
}

TableControl::~TableControl() {
  if (_column_headers_control) {
    delete _column_headers_control;
    _column_headers_control = nullptr;
  }

  if (_row_headers_control) {
    delete _row_headers_control;
    _row_headers_control = nullptr;
  }
}

void TableControl::Init() {
  Bind(wxEVT_SET_FOCUS, &TableControl::OnSetFocus, this);

  CreateHeaderControls();
  CreateCellsViewControl();

  // Create Sizer which includes Headers at top and left side
  _top_sizer = new wxBoxSizer(wxVERTICAL);

  _top_sizer->Add(_column_headers_control, 0, wxEXPAND | wxALL, 0);

  _row_cell_view_sizer = new wxBoxSizer(wxHORIZONTAL);
  _row_cell_view_sizer->Add(_row_headers_control, 0, wxEXPAND, 0);
  _row_cell_view_sizer->Add(_cells_view_control, 1, wxEXPAND, 0);

  _top_sizer->Add(_row_cell_view_sizer, 1, wxEXPAND | wxALL, 0);

  SetSizerAndFit(_top_sizer);

  _cells_view_control->SetFocus();
}

void TableControl::CreateHeaderControls() {
  _row_headers_control = new TableRowHeadersControl(
      _sys_colors, _document, _event_sink, this, wxID_ANY, wxDefaultPosition,
      wxDefaultSize, wxWANTS_CHARS);
  _column_headers_control = new TableColumnHeadersControl(
      _sys_colors, _document, _event_sink, this, wxID_ANY, wxDefaultPosition,
      wxDefaultSize, wxWANTS_CHARS);
}

void TableControl::CreateCellsViewControl() {
  _cells_view_control =
      new CellsViewControl(_sys_colors, _document, _event_sink, this, wxID_ANY,
                           wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS);
  _cells_view_control->Initialize();
}

void TableControl::OnCellUpdate(const Location &location) {
  _cells_view_control->OnCellUpdate(location);
}

void TableControl::OnSetFocus(wxFocusEvent &event) {
  std::ignore = event;
  _cells_view_control->SetFocus();
}

void TableControl::update_scroll_positions(const Location &scroll_pos) {
  _row_headers_control->Scroll(0, scroll_pos.y());
  _column_headers_control->Scroll(scroll_pos.x(), 0);
}

void TableControl::ScrollToCurrentCell(CellWindowLocation location) {
  _cells_view_control->ScrollToCurrentCell(location);
}
