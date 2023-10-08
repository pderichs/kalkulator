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

#ifndef TABLE_SEARCH_RESULTS_DIALOG_H
#define TABLE_SEARCH_RESULTS_DIALOG_H

#include "model/event_sink.h"
#include "model/table/table_search_result.h"
#include "tools.h"
#include <wx/image.h>
#include <wx/listctrl.h>
#include <wx/wx.h>

class TableSearchResultsDlg : public wxFrame {
public:
  TableSearchResultsDlg(wxWindow *parent, EventSink *event_sink,
                        wxWindowID id = wxID_ANY,
                        const wxString &title = wxT("Search Results"),
                        const wxPoint &pos = wxDefaultPosition,
                        const wxSize &size = wxDefaultSize,
                        long style = wxDEFAULT_FRAME_STYLE);

  // Delete copy constructor and assignment operator
  TableSearchResultsDlg(const TableSearchResultsDlg &other) = delete;
  TableSearchResultsDlg &operator=(const TableSearchResultsDlg &other) = delete;

  void Initialize(const TableSearchResult &results);

  void OnClose(wxCommandEvent &event);
  void OnListItemActivated(wxListEvent &event);

private:
  TableSearchResult _results;
  wxListCtrl *_lst_results;
  wxButton *_btn_close;
  EventSink *_event_sink;
};

#endif
