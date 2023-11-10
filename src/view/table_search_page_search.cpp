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

#include "table_search_page_search.h"
#include "search_text_control.h"

TableSearchPageSearch::TableSearchPageSearch(wxWindow *parent,
                                             const TableWorkbookDocumentPtr &document,
                                             EventSink *event_sink,
                                             wxWindowID id,
                                             const wxPoint &pos,
                                             const wxSize &size,
                                             long style,
                                             const wxString &name) : wxPanel(
    parent,
    id,
    pos,
    size,
    style,
    name), _event_sink(event_sink), _results(), _lst_results(nullptr),
                                                                     _txt_search_term(
                                                                         nullptr),
                                                                     _btn_search(
                                                                         nullptr),
                                                                     _document(
                                                                         document) {
  wxBoxSizer *sizer_1 = new wxBoxSizer(wxVERTICAL);

  wxBoxSizer *text_search_sizer = new wxBoxSizer(wxHORIZONTAL);
  text_search_sizer->Add(new wxStaticText(this, wxID_ANY, "Search term:"),
                         0,
                         wxALIGN_LEFT | wxALL,
                         5);
  _txt_search_term = new SearchTextControl(this, this, wxID_ANY);
  text_search_sizer->Add(_txt_search_term, 1, wxEXPAND | wxALL, 5);
  sizer_1->Add(text_search_sizer, 0, wxEXPAND, 5);

  _btn_search = new wxButton(this, wxID_ANY);
  _btn_search->SetLabelText("Search");

  _btn_search->Bind(wxEVT_BUTTON, &TableSearchPageSearch::OnSearch, this);
  sizer_1->Add(_btn_search, 0, wxALIGN_RIGHT | wxALL, 5);

  _lst_results = CreateResultsListView();
  sizer_1->Add(_lst_results, 1, wxEXPAND | wxALL, 5);

  SetSizer(sizer_1);
  sizer_1->Fit(this);
}

wxListCtrl *TableSearchPageSearch::CreateResultsListView() {
  auto result = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                               wxLC_HRULES | wxLC_REPORT | wxLC_VRULES);
  result->AppendColumn(wxT("Sheet"), wxLIST_FORMAT_LEFT, -1);
  result->AppendColumn(wxT("Location"), wxLIST_FORMAT_LEFT, -1);
  result->AppendColumn(wxT("Content"), wxLIST_FORMAT_LEFT, -1);
  result->Bind(wxEVT_COMMAND_LIST_ITEM_ACTIVATED,
               &TableSearchPageSearch::OnListItemActivated, this);

  return result;
}

void TableSearchPageSearch::OnListItemActivated(wxListEvent &event) {
  long item_index = event.GetIndex();
  auto result = _results[item_index];

  _event_sink->send_event(NAVIGATE_SEARCH_RESULT_ITEM, result);
}

void TableSearchPageSearch::SetResults(const TableSearchResult &results) {
  _results = results;

  _lst_results->DeleteAllItems();
  for (const auto &result : _results) {
    long index = _lst_results->InsertItem(0, result.sheet->name());
    _lst_results->SetItem(index, 1, pdtools::locationToString(result.location));

    auto cell = result.sheet->get_cell(result.location);
    _lst_results->SetItem(index, 2, cell->visible_content());
  }
}

void TableSearchPageSearch::OnSearch(wxCommandEvent &WXUNUSED(event)) {
  SearchTerm();
}

void TableSearchPageSearch::SearchTerm() {
  wxString search_term(_txt_search_term->GetValue());

  if (search_term.IsEmpty()) {
    _txt_search_term->SetFocus();
    return;
  }

  TableSearchResult search_result =
      _document->search_sheets(static_cast<const char *>(search_term));

  if (search_result.empty()) {
    wxMessageBox("No occurrences found.", "Search");
    return;
  }

  SetResults(search_result);
}

void TableSearchPageSearch::send_event(TableEvent event_id, std::any param) {
  std::ignore = param;

  if (event_id == SEARCH_TEXT_CONTROL_SEARCH_TERM_COMMAND) {
    SearchTerm();
  }
}
