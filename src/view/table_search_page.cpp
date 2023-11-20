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

#include "table_search_page.h"
#include "search_text_control.h"

TableSearchPage::TableSearchPage(wxWindow *parent,
                                 const TableWorkbookDocumentPtr &document,
                                 EventSink *event_sink,
                                 wxWindowID id,
                                 const wxPoint &pos,
                                 const wxSize &size,
                                 long style,
                                 const wxString &name)
    : wxPanel(parent, id, pos, size, style, name), _event_sink(event_sink),
      _document(document), _lst_results(nullptr), _results(),
      _txt_search_term(nullptr), _btn_search(nullptr) {}

void TableSearchPage::CreateResultsListView() {
  _lst_results =
      new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                     wxLC_HRULES | wxLC_REPORT | wxLC_VRULES);
  _lst_results->AppendColumn(wxT("Sheet"), wxLIST_FORMAT_LEFT, -1);
  _lst_results->AppendColumn(wxT("Location"), wxLIST_FORMAT_LEFT, -1);
  _lst_results->AppendColumn(wxT("Content"), wxLIST_FORMAT_LEFT, -1);
  _lst_results->Bind(wxEVT_COMMAND_LIST_ITEM_ACTIVATED,
                     &TableSearchPage::OnListItemActivated, this);
}

void TableSearchPage::OnListItemActivated(wxListEvent &event) {
  long selection_index = event.GetIndex();
  long item_index = _lst_results->GetItemData(selection_index);

  auto result = _results[item_index];

  _event_sink->send_event(NAVIGATE_SEARCH_RESULT_ITEM, result);
}

void TableSearchPage::SetResults(const TableSearchResult &results) {
  _results = results;

  _lst_results->DeleteAllItems();

  size_t item_index = 0;
  for (const auto &result : _results) {
    long index = _lst_results->InsertItem(0, result.sheet->name());
    _lst_results->SetItem(index, 1, pdtools::locationToString(result.location));

    auto cell = result.sheet->get_cell(result.location);
    _lst_results->SetItem(index, 2, cell->visible_content());

    _lst_results->SetItemData(index, item_index);

    item_index++;
  }
}

void TableSearchPage::CreateSearchTextControl() {
  _txt_search_term = new SearchTextControl(this, this, wxID_ANY);
}

void TableSearchPage::SearchTerm() {
  _lst_results->DeleteAllItems();

  wxString search_term(_txt_search_term->GetValue());

  if (search_term.IsEmpty()) {
    _txt_search_term->SetFocus();
    return;
  }

  TableSearchResult search_result =
      _document->search_sheets(static_cast<const char *>(search_term));

  if (search_result.empty()) {
    return;
  }

  SetResults(search_result);
}

void TableSearchPage::send_event(TableEvent event_id, std::any param) {
  std::ignore = param;

  if (event_id == SEARCH_TEXT_CONTROL_SEARCH_TERM_COMMAND) {
    SearchTerm();
  }
}

void TableSearchPage::OnSearch(wxCommandEvent &WXUNUSED(event)) {
  SearchTerm();
}

