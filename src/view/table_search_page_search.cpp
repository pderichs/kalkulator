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
                                             EventSink *event_sink)
    : TableSearchPage(parent, document, event_sink) {
  wxBoxSizer *sizer_1 = new wxBoxSizer(wxVERTICAL);

  wxBoxSizer *text_search_sizer = new wxBoxSizer(wxHORIZONTAL);
  text_search_sizer->Add(new wxStaticText(this, wxID_ANY, "Search term:"),
                         0,
                         wxALIGN_LEFT | wxALL,
                         5);
  CreateSearchTextControl();
  text_search_sizer->Add(_txt_search_term, 1, wxEXPAND | wxALL, 5);
  sizer_1->Add(text_search_sizer, 0, wxEXPAND, 5);

  _btn_search = new wxButton(this, wxID_ANY);
  _btn_search->SetLabelText("Search");

  _btn_search->Bind(wxEVT_BUTTON, &TableSearchPageSearch::OnSearch, this);
  sizer_1->Add(_btn_search, 0, wxALIGN_RIGHT | wxALL, 5);

  CreateResultsListView();
  sizer_1->Add(_lst_results, 1, wxEXPAND | wxALL, 5);

  SetSizer(sizer_1);
  sizer_1->Fit(this);
}

