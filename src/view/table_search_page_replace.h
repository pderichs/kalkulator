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

#ifndef TABLE_SEARCH_PAGE_REPLACE_H
#define TABLE_SEARCH_PAGE_REPLACE_H

#include "table_search_page.h"
#include "model/table/table_search_result.h"

/**
 * Combined search / replace page in search dialog.
 */
class TableSearchPageReplace : public TableSearchPage {
public:
  TableSearchPageReplace(wxWindow *parent,
                         const TableWorkbookDocumentPtr &document,
                         EventSink *event_sink);

  // Delete copy constructor and assignment operator
  TableSearchPageReplace(const TableSearchPageReplace &other) = delete;
  TableSearchPageReplace &operator=(const TableSearchPageReplace &other) = delete;

  void OnReplaceSelected(wxCommandEvent &);
  void OnReplaceAll(wxCommandEvent &event);

private:
  void ReplaceItem(long index, UpdateIdType update_id);

private:
  wxTextCtrl *_txt_replace_term;
  wxButton *_btn_replace_current;
  wxButton *_btn_replace_all;
};

#endif //TABLE_SEARCH_PAGE_REPLACE_H
