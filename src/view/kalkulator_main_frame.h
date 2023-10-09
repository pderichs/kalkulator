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

#ifndef KALKULATOR_MAIN_FRAME_H
#define KALKULATOR_MAIN_FRAME_H

#include "model/event_sink.h"
#include "model/lisp/lisp_execution_context.h"
#include "model/table/table_workbook_document.h"
#include "view/kalkulator_system_colors.h"
#include "view/table_search_dlg.h"
#include "view/table_cell_format_dlg.h"
#include "view/table_control.h"

#if !defined(WX_PRECOMP)
#include <wx/wx.h>
#endif

/**
 * Provides the main frame functionality of the application.
 * It also serves as the main event sink.
 */
class KalkulatorMainFrame : public wxFrame, public EventSink {
public:
  KalkulatorMainFrame();
  ~KalkulatorMainFrame() override;

  void Initialize();

  // Delete copy constructor and assignment operator
  KalkulatorMainFrame(const KalkulatorMainFrame &other) = delete;
  KalkulatorMainFrame &operator=(const KalkulatorMainFrame &other) = delete;

  virtual void OnClose(wxCloseEvent &event);

private:
  void OnExit(wxCommandEvent &event);
  void OnAbout(wxCommandEvent &event);
  void OnOpen(wxCommandEvent &event);
  void OnNew(wxCommandEvent &event);
  void OnSave(wxCommandEvent &event);
  void OnSaveAs(wxCommandEvent &event);
  void OnRightDown(wxMouseEvent &event);
  void OnSheetSelectionCombo(wxCommandEvent &event);
  void OnResizeColumn(wxCommandEvent &event);
  void OnResizeRow(wxCommandEvent &event);
  void OnGotoCell(wxCommandEvent &event);
  void OnFormatCell(wxCommandEvent &event);
  void OnAddSheet(wxCommandEvent &WXUNUSED(event));
  void OnRemoveSheet(wxCommandEvent &WXUNUSED(event));
  void OnSearch(wxCommandEvent &WXUNUSED(event));

  void BindEvents();
  void CreateToolbar();

  void InitializeModel();
  void InitializeMenu();
  void InitializeIcons();

  void UpdateSheetCombo();
  void SetupUserInterface();
  void send_event(TableEvent event_id, std::any param) override;

  bool PermitLoseChanges();

  static bool IsDarkUI() {
    wxSystemAppearance s = wxSystemSettings::GetAppearance();
    return s.IsDark();
  }

  const char **GetIcon(const std::string &icon_key) const;

  void SaveDocument(const std::string &file_path);

  void UpdateFormulaBySelectedCell(const Location &location);

private:
  TableWorkbookDocumentPtr _document;
  TableControl *_table_control;
  wxTextCtrl *_text_control_formula;
  LispExecutionContext _execution_context;
  KalkulatorSystemColorsPtr _sys_colors;

  wxToolBar *_toolbar;
  wxButton *_btn_formula_selection;
  wxComboBox *_cmb_sheet_selection;

  wxBitmap *_icon_new;
  wxBitmap *_icon_open;
  wxBitmap *_icon_save;
  wxBitmap *_icon_height;
  wxBitmap *_icon_width;

  wxString _last_search_term;
  TableSearchResultsDlg *_search_results_dlg;
};

enum {
  ID_Open = 1,
  ID_Save,
  ID_SaveAs,
  ID_New,
  ID_ResizeColumn,
  ID_ResizeRow,
  ID_GotoCell,
  ID_FormatCell,
  ID_AddSheet,
  ID_RemoveSheet,
  ID_Copy,
  ID_Paste,
  ID_Search,
};

enum { ID_SHEET_SELECTION_CMB = wxID_HIGHEST + 1 };

#endif //KALKULATOR_MAIN_FRAME_H
