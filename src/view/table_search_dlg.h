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
#include "model/table/table_workbook_document.h"
#include <wx/image.h>
#include <wx/listctrl.h>
#include <wx/notebook.h>
#include <wx/wx.h>

/**
 * Implementation of the search dialog.
 */
class TableSearchResultsDlg : public wxFrame {
public:
  TableSearchResultsDlg(wxWindow *parent,
                        EventSink *event_sink,
                        const TableWorkbookDocumentPtr &document,
                        wxWindowID id = wxID_ANY,
                        const wxString &title = wxT("Search and Replace"),
                        const wxPoint &pos = wxDefaultPosition,
                        const wxSize &size = wxDefaultSize,
                        long style = wxDEFAULT_FRAME_STYLE);

  // Delete copy constructor and assignment operator
  TableSearchResultsDlg(const TableSearchResultsDlg &other) = delete;
  TableSearchResultsDlg &operator=(const TableSearchResultsDlg &other) = delete;

  void OnClose(wxCommandEvent &event);

private:
  wxButton *CreateCloseButton();
  wxNotebook *CreateNotebookControl();

private:
  wxButton *_btn_close;
  EventSink *_event_sink;
  wxNotebook *_notebook;
  TableWorkbookDocumentPtr _document;
};

#endif
