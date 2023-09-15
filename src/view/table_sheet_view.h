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


#ifndef TABLE_SHEET_VIEW_INCLUDED
#define TABLE_SHEET_VIEW_INCLUDED

#include "../model/table/table_workbook_document.h"
#include "tools.h"
#include <wx/wx.h>

class TableSheetView : public wxScrolledWindow {
public:
  TableSheetView(TableWorkbookDocumentPtr document, EventSink *event_sink,
                 wxWindow *parent, wxWindowID id, const wxPoint &pos,
                 const wxSize &size, long style);
  virtual ~TableSheetView() = default;

  Location GetScrollPosition() const;
  void DrawTextInCenter(wxDC *dc, const wxString &s, const wxRect &rect);

protected:
  TableWorkbookDocumentPtr _document;
  EventSink *_event_sink;
};

#endif
