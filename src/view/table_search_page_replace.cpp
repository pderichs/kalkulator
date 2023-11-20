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

#include "table_search_page_replace.h"
#include "search_text_control.h"

TableSearchPageReplace::TableSearchPageReplace(wxWindow *parent,
                                               const TableWorkbookDocumentPtr &document,
                                               EventSink *event_sink)
    : TableSearchPage(parent, document, event_sink),
      _txt_replace_term(nullptr), _btn_replace_current(nullptr),
      _btn_replace_all(nullptr) {
  wxBoxSizer *sizer_1 = new wxBoxSizer(wxVERTICAL);

  wxBoxSizer *text_search_sizer = new wxBoxSizer(wxHORIZONTAL);
  text_search_sizer->Add(new wxStaticText(this, wxID_ANY, "Search term:"),
                         0,
                         wxALIGN_LEFT | wxALL,
                         5);
  CreateSearchTextControl();
  text_search_sizer->Add(_txt_search_term, 1, wxEXPAND | wxALL, 5);
  sizer_1->Add(text_search_sizer, 0, wxEXPAND, 5);

  wxBoxSizer *text_replace_sizer = new wxBoxSizer(wxHORIZONTAL);
  text_replace_sizer->Add(new wxStaticText(this, wxID_ANY, "Replace with:"),
                          0,
                          wxALIGN_LEFT | wxALL,
                          5);
  _txt_replace_term = new SearchTextControl(this, this, wxID_ANY);
  text_replace_sizer->Add(_txt_replace_term, 1, wxEXPAND | wxALL, 5);
  sizer_1->Add(text_replace_sizer, 0, wxEXPAND, 5);

  _btn_search = new wxButton(this, wxID_ANY);
  _btn_search->SetLabelText("Search");
  _btn_search->Bind(wxEVT_BUTTON, &TableSearchPageReplace::OnSearch, this);
  sizer_1->Add(_btn_search, 0, wxALIGN_RIGHT | wxALL, 5);

  CreateResultsListView();
  sizer_1->Add(_lst_results, 1, wxEXPAND | wxALL, 5);

  wxBoxSizer *replace_buttons_sizer = new wxBoxSizer(wxHORIZONTAL);
  _btn_replace_current = new wxButton(this, wxID_ANY);
  _btn_replace_current->SetLabelText("Replace selected Items");
  _btn_replace_current->Bind(wxEVT_BUTTON,
                             &TableSearchPageReplace::OnReplaceSelected,
                             this);
  replace_buttons_sizer->Add(_btn_replace_current, 0, wxALL, 5);

  _btn_replace_all = new wxButton(this, wxID_ANY);
  _btn_replace_all->SetLabelText("Replace all occurrences");
  _btn_replace_all->Bind(wxEVT_BUTTON,
                         &TableSearchPageReplace::OnReplaceAll,
                         this);
  replace_buttons_sizer->Add(_btn_replace_all, 0, wxALL, 5);

  sizer_1->Add(replace_buttons_sizer, 0, wxEXPAND | wxALL, 5);

  SetSizer(sizer_1);
  sizer_1->Fit(this);
}

void TableSearchPageReplace::OnReplaceSelected(wxCommandEvent &
                                               WXUNUSED(event)) {
  if (_lst_results->IsEmpty()) {
    return;
  }

  std::set<long> replaced_item_indexes;
  UpdateIdType update_id = generate_update_id();

  long index = -1;
  while (true) {
    index = _lst_results->GetNextItem(index,
                                      wxLIST_NEXT_ALL,
                                      wxLIST_STATE_SELECTED);
    if (index == wxNOT_FOUND) {
      break;
    }

    ReplaceItem(index, update_id);
  }

  SearchTerm();
}

void TableSearchPageReplace::OnReplaceAll(wxCommandEvent &WXUNUSED(event)) {
  if (_lst_results->IsEmpty()) {
    return;
  }

  UpdateIdType update_id = generate_update_id();

  for (long n = 0; n < _lst_results->GetItemCount(); n++) {
    ReplaceItem(n, update_id);
  }

  SearchTerm();
}

void TableSearchPageReplace::ReplaceItem(long index, UpdateIdType update_id) {
  long item_index = _lst_results->GetItemData(index);
  const auto &result_item = _results[item_index];

  _document->replace_search_item(result_item,
                                 static_cast<std::string>(_txt_search_term->GetValue()),
                                 static_cast<std::string>(_txt_replace_term->GetValue()),
                                 update_id);
}
