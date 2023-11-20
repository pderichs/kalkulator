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
#include "table_search_page_search.h"
#include "table_search_page_replace.h"

TableSearchResultsDlg::TableSearchResultsDlg(
    wxWindow *parent,
    EventSink *event_sink,
    const TableWorkbookDocumentPtr &document,
    wxWindowID id,
    const wxString &title,
    const wxPoint &pos,
    const wxSize &size,
    long style)
    : wxFrame(parent, id, title, pos, size, style),
      _btn_close(nullptr), _event_sink(event_sink),
      _notebook(nullptr), _document(document) {
  wxBoxSizer *main_sizer = new wxBoxSizer(wxVERTICAL);

  _notebook = CreateNotebookControl();
  main_sizer->Add(_notebook, 1, wxEXPAND, 5);

  _btn_close = CreateCloseButton();
  main_sizer->Add(_btn_close, 0, wxALIGN_RIGHT, 5);

  SetSizer(main_sizer);
  main_sizer->Fit(this);

  SetSize(800, 600);
}

void TableSearchResultsDlg::OnClose(wxCommandEvent &WXUNUSED(event)) {
  // NOTE: This will also delete the dialog.
  Close();
}

wxButton *TableSearchResultsDlg::CreateCloseButton() {
  auto result = new wxButton(this, wxID_ANY, wxT("Close"));
  result->Bind(wxEVT_BUTTON, &TableSearchResultsDlg::OnClose, this);
  return result;
}

wxNotebook *TableSearchResultsDlg::CreateNotebookControl() {
  auto result = new wxNotebook(this, wxID_ANY);
  result->AddPage(new TableSearchPageSearch(result, _document, _event_sink),
                  "Search",
                  true);
  result->AddPage(new TableSearchPageReplace(result, _document, _event_sink),
                  "Replace");

  return result;
}
