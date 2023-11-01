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

#include "table_search_dlg.h"

TableSearchResultsDlg::TableSearchResultsDlg(
    wxWindow *parent, EventSink *event_sink, wxWindowID id,
    const wxString &title, const wxPoint &pos, const wxSize &size, long style)
    : wxFrame(parent, id, title, pos, size, style), _results(),
      _lst_results(nullptr), _btn_close(nullptr), _event_sink(event_sink) {
  wxBoxSizer *sizer_1 = new wxBoxSizer(wxVERTICAL);
  _lst_results =
      new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                     wxLC_HRULES | wxLC_REPORT | wxLC_VRULES);
  _lst_results->AppendColumn(wxT("Sheet"), wxLIST_FORMAT_LEFT, -1);
  _lst_results->AppendColumn(wxT("Location"), wxLIST_FORMAT_LEFT, -1);
  _lst_results->AppendColumn(wxT("Content"), wxLIST_FORMAT_LEFT, -1);
  _lst_results->Bind(wxEVT_COMMAND_LIST_ITEM_ACTIVATED,
                     &TableSearchResultsDlg::OnListItemActivated, this);
  sizer_1->Add(_lst_results, 1, wxEXPAND, 5);

  _btn_close = new wxButton(this, wxID_ANY, wxT("Close"));
  _btn_close->Bind(wxEVT_BUTTON, &TableSearchResultsDlg::OnClose, this);
  sizer_1->Add(_btn_close, 0, wxALIGN_RIGHT, 5);

  SetSizer(sizer_1);
  sizer_1->Fit(this);
}

void TableSearchResultsDlg::Initialize(const TableSearchResult &results) {
  _results = results;

  Layout();

  _lst_results->DeleteAllItems();
  for (const auto &result : _results) {
    long index = _lst_results->InsertItem(0, result.sheet->name());
    _lst_results->SetItem(index, 1, pdtools::locationToString(result.location));

    auto cell = result.sheet->get_cell(result.location);
    _lst_results->SetItem(index, 2, cell->visible_content());
  }
}

void TableSearchResultsDlg::OnClose(wxCommandEvent &WXUNUSED(event)) {
  // NOTE: This will also delete the dialog.
  Close();
}

void TableSearchResultsDlg::OnListItemActivated(wxListEvent &event) {
  long item_index = event.GetIndex();
  auto result = _results[item_index];

  _event_sink->send_event(NAVIGATE_SEARCH_RESULT_ITEM, result);
}
