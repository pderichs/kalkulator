#include "table_control.h"
#include "table_column_headers_control.h"
#include "table_row_headers_control.h"
#include <cstddef>
#include <iostream>
#include <sstream>
#include <tuple>
#include <wx/clipbrd.h>
#include <wx/dcclient.h>

TableControl::TableControl(KalkulatorSystemColorsPtr sys_colors,
                           TableWorkbookDocumentPtr document,
                           EventSink *event_sink, wxWindow *parent,
                           wxWindowID id, const wxPoint &pos,
                           const wxSize &size, long style)
    : wxWindow(parent, id, pos, size, style) {
  assert(event_sink);
  assert(document);

  _event_sink = event_sink;
  _document = document;

  _sys_colors = sys_colors;

  Init();
}

TableControl::~TableControl() {
  if (_column_headers_control) {
    delete _column_headers_control;
    _column_headers_control = NULL;
  }

  if (_row_headers_control) {
    delete _row_headers_control;
    _row_headers_control = NULL;
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
