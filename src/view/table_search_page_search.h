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

#ifndef TABLE_SEARCH_PAGE_SEARCH_H
#define TABLE_SEARCH_PAGE_SEARCH_H

#include <wx/listctrl.h>
#include <wx/panel.h>
#include <wx/wx.h>
#include "model/table/table_search_result.h"
#include "model/event_sink.h"
#include "model/table/table_workbook_document.h"

class TableSearchPageSearch : public wxPanel, public EventSink {
public:
  TableSearchPageSearch(wxWindow *parent,
                        const TableWorkbookDocumentPtr &document,
                        EventSink *event_sink,
                        wxWindowID id = wxID_ANY,
                        const wxPoint &pos = wxDefaultPosition,
                        const wxSize &size = wxDefaultSize,
                        long style = wxTAB_TRAVERSAL,
                        const wxString &name = wxPanelNameStr);

  // Delete copy constructor and assignment operator
  TableSearchPageSearch(const TableSearchPageSearch &other) = delete;
  TableSearchPageSearch &operator=(const TableSearchPageSearch &other) = delete;

  void OnListItemActivated(wxListEvent &event);

  void OnSearch(wxCommandEvent &event);

  void send_event(TableEvent event_id, std::any param) override;

private:
  wxListCtrl *CreateResultsListView();
  void SetResults(const TableSearchResult &results);
  void SearchTerm();

private:
  EventSink *_event_sink;
  TableSearchResult _results;
  wxListCtrl *_lst_results;
  wxTextCtrl *_txt_search_term;
  wxButton *_btn_search;
  TableWorkbookDocumentPtr _document;
};

#endif //TABLE_SEARCH_PAGE_SEARCH_H
