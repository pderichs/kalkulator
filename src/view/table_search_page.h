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

#ifndef TABLE_SEARCH_PAGE_H
#define TABLE_SEARCH_PAGE_H

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/listctrl.h>
#include "model/event_sink.h"
#include "model/table/table_search_result.h"
#include "model/table/table_workbook_document.h"

/**
 * Base class for pages in search dialog.
 */
class TableSearchPage : public wxPanel, public EventSink {
public:
  TableSearchPage(wxWindow *parent,
                  const TableWorkbookDocumentPtr &document,
                  EventSink *event_sink,
                  wxWindowID id = wxID_ANY,
                  const wxPoint &pos = wxDefaultPosition,
                  const wxSize &size = wxDefaultSize,
                  long style = wxTAB_TRAVERSAL,
                  const wxString &name = wxPanelNameStr);

  // Delete copy constructor and assignment operator
  TableSearchPage(const TableSearchPage &other) = delete;
  TableSearchPage &operator=(const TableSearchPage &other) = delete;

  void OnSearch(wxCommandEvent &event);

  void OnListItemActivated(wxListEvent &event);
  void send_event(TableEvent event_id, std::any param) override;

protected:
  void CreateResultsListView();
  void CreateSearchTextControl();
  void SetResults(const TableSearchResult &results);
  void SearchTerm();

protected:
  EventSink *_event_sink;
  TableWorkbookDocumentPtr _document;
  wxListCtrl *_lst_results;
  TableSearchResult _results;
  wxTextCtrl *_txt_search_term;
  wxButton *_btn_search;
};

#endif //TABLE_SEARCH_PAGE_H
