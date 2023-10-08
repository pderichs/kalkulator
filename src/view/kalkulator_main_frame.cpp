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

#include "kalkulator_main_frame.h"
#include "model/table/lisp_execution_context_cell_range.h"
#include "model/table/lisp_execution_context_cell_reference.h"
#include "model/table/lisp_execution_context_message_box.h"
#include "model/table/table_workbook_file.h"
#include "model/table/table_workbook_file_error.h"
#include "view/icons.h"
#include "view/table_formula_text_control.h"
#include <iostream>
#include <memory>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <tuple>
#include <wx/aboutdlg.h>
#include <wx/event.h>
#include <wx/filename.h>
#include <wx/msgdlg.h>
#include <wx/numdlg.h>
#include <wx/rawbmp.h>
#include <wx/stdpaths.h>
#include <wx/wxprec.h>

#define VERSION "0.0.1a"

typedef std::pair<const char **, const char **> IconPaths;

// clang-format off
// Map between icon_key and dark mode and bright mode icon paths.
std::map<std::string, IconPaths> IconDictionary = {
    {"new", {outline_insert_drive_file_white_18dp_xpm,
             outline_insert_drive_file_black_18dp_xpm}},
    {"open",
     {outline_folder_open_white_18dp_xpm, outline_folder_open_black_18dp_xpm}},
    {"save", {outline_save_white_18dp_xpm, outline_save_black_18dp_xpm}},
    {"height", {outline_height_white_18dp_xpm, outline_height_black_18dp_xpm}}
};
// clang-format on

KalkulatorMainFrame::KalkulatorMainFrame()
    : wxFrame(nullptr, wxID_ANY, "Kalkulator " VERSION),
      _document(std::make_shared<TableWorkbookDocument>(this)),
      _table_control(nullptr), _text_control_formula(nullptr),
      _execution_context(), _sys_colors(), _toolbar(nullptr),
      _btn_formula_selection(nullptr), _cmb_sheet_selection(nullptr),
      _icon_new(nullptr), _icon_open(nullptr), _icon_save(nullptr),
      _icon_height(nullptr), _icon_width(nullptr), _last_search_term(),
      _search_results_dlg(nullptr) {
  InitializeModel();
  InitializeIcons();
  InitializeMenu();
  BindEvents();
  CreateToolbar();
  SetupUserInterface();
}

KalkulatorMainFrame::~KalkulatorMainFrame() {
  if (_icon_new) {
    delete _icon_new;
    _icon_new = nullptr;
  }

  if (_icon_height) {
    delete _icon_height;
    _icon_height = nullptr;
  }

  if (_icon_width) {
    delete _icon_width;
    _icon_width = nullptr;
  }

  if (_icon_open) {
    delete _icon_open;
    _icon_open = nullptr;
  }

  if (_icon_save) {
    delete _icon_save;
    _icon_save = nullptr;
  }

  if (_toolbar) {
    delete _toolbar;
    _toolbar = nullptr;
  }

  if (_table_control) {
    delete _table_control;
    _table_control = nullptr;
  }

  if (_text_control_formula) {
    delete _text_control_formula;
    _text_control_formula = nullptr;
  }

  if (_search_results_dlg) {
    delete _search_results_dlg;
    _search_results_dlg = nullptr;
  }
}

void KalkulatorMainFrame::Initialize() {
  CreateStatusBar();
  SetStatusText("Welcome to Kalkulator!");
}

void KalkulatorMainFrame::InitializeIcons() {
  _icon_new = new wxBitmap(GetIcon("new"));
  _icon_open = new wxBitmap(GetIcon("open"));
  _icon_save = new wxBitmap(GetIcon("save"));
  _icon_height = new wxBitmap(GetIcon("height"));

  wxBitmap temp(GetIcon("height"));
  wxImage image = temp.ConvertToImage();
  image = image.Rotate90();
  _icon_width = new wxBitmap(image);
}

void KalkulatorMainFrame::InitializeModel() {
  ValueConverter::set_execution_context(&_execution_context);

  _execution_context.add_function(
      "cell", std::make_shared<LispExecutionContextCellReference>(_document));

  auto cell_range = std::make_shared<LispExecutionContextCellRange>(_document);
  _execution_context.add_function("cell_range", cell_range);
  _execution_context.add_function("cell-range", cell_range);
  _execution_context.add_function("message-box",
                                  std::make_shared<
                                      LispExecutionContextMessageBox>(this));

  _sys_colors = std::make_shared<KalkulatorSystemColors>();

  _table_control = new TableControl(_sys_colors, _document, this, this,
                                    wxID_ANY, wxDefaultPosition, wxDefaultSize);

  _text_control_formula = new TableFormulaTextControl(
      this, this, -1, "", wxDefaultPosition, wxDefaultSize);
}

void KalkulatorMainFrame::InitializeMenu() {
  wxMenuItem *item;

  auto *menuFile = new wxMenu();
  item = new wxMenuItem(menuFile, ID_New, "&New\tCtrl-N",
                        "Creates a new spreadsheet workbook");
  item->SetBitmap(*_icon_new);
  menuFile->Append(item);

  item = new wxMenuItem(menuFile, ID_Open, "&Open...\tCtrl-O",
                        "Opens a Kalkulator file");
  item->SetBitmap(*_icon_open);
  menuFile->Append(item);

  item = new wxMenuItem(menuFile, ID_Save, "&Save\tCtrl-S",
                        "Saves the current workbook");
  item->SetBitmap(*_icon_save);
  menuFile->Append(item);

  menuFile->Append(
      ID_SaveAs, "&Save as...",
      "Saves the current workbook under a specified file name and path");

  menuFile->AppendSeparator();

  menuFile->Append(wxID_EXIT);

  auto *menuEdit = new wxMenu();
  item = new wxMenuItem(menuEdit, ID_Copy, "&Copy\tCtrl+C",
                        "Copies the current cell content");
  menuEdit->Append(item);
  item =
      new wxMenuItem(menuEdit, ID_Paste, "&Paste\tCtrl+V",
                     "Pastes the content of the clipboard to the current cell");
  menuEdit->Append(item);
  menuEdit->AppendSeparator();
  item = new wxMenuItem(menuEdit, ID_Search, "&Search\tCtrl+F",
                        "Search for a string");
  menuEdit->Append(item);

  auto *menuSheets = new wxMenu();
  item = new wxMenuItem(menuSheets, ID_AddSheet, "Add Sheet...",
                        "Adds a new sheet to the workbook");
  menuSheets->Append(item);
  item = new wxMenuItem(menuSheets, ID_RemoveSheet, "Remove Sheet",
                        "Removes the current sheet from the workbook");
  menuSheets->Append(item);

  auto *menuTable = new wxMenu();
  item = new wxMenuItem(menuTable, ID_ResizeColumn, "Resize column...",
                        "Provides possibility to change current column width.");
  item->SetBitmap(*_icon_width);
  menuTable->Append(item);
  item = new wxMenuItem(menuTable, ID_ResizeRow, "Resize row...",
                        "Provides possibility to change current row height.");
  item->SetBitmap(*_icon_height);
  menuTable->Append(item);

  menuTable->AppendSeparator();

  item = new wxMenuItem(menuTable, ID_GotoCell, "&Goto cell...\tCtrl-G",
                        "Select cell by entering the desired coordinates.");
  menuTable->Append(item);

  item = new wxMenuItem(menuTable, ID_FormatCell, "&Format Cell...",
                        "Edit format options for current cell.");
  menuTable->Append(item);

  auto *menuHelp = new wxMenu();
  menuHelp->Append(wxID_ABOUT);

  auto *menuBar = new wxMenuBar();
  menuBar->Append(menuFile, "&File");
  menuBar->Append(menuEdit, "&Edit");
  menuBar->Append(menuSheets, "&Sheets");
  menuBar->Append(menuTable, "&Table");
  menuBar->Append(menuHelp, "&Help");
  SetMenuBar(menuBar);
}

const char **KalkulatorMainFrame::GetIcon(const std::string &icon_key) const {
  auto it = IconDictionary.find(icon_key);
  if (it == IconDictionary.end()) {
    std::stringstream ss;
    ss << "Icon ";
    ss << icon_key << " does not exist.";
    throw std::runtime_error(ss.str());
  }

  const auto &icons = it->second;

  const char **icon;
  if (IsDarkUI()) {
    icon = icons.first;
  } else {
    icon = icons.second;
  }

  return icon;
}

void KalkulatorMainFrame::CreateToolbar() {
  _toolbar = new wxToolBar(this, wxID_ANY);
  _toolbar->AddTool(ID_New, wxT("Create new document"), *_icon_new);
  _toolbar->AddTool(ID_Open, wxT("Open an existing document"), *_icon_open);
  _toolbar->AddTool(ID_Save, wxT("Save document"), *_icon_save);
  _toolbar->AddSeparator();
  _toolbar->AddTool(ID_ResizeColumn, wxT("Resize column"), *_icon_width);
  _toolbar->AddTool(ID_ResizeRow, wxT("Resize row"), *_icon_height);
}

void KalkulatorMainFrame::SetupUserInterface() {
  auto *sizer = new wxBoxSizer(wxVERTICAL);

  sizer->Add(_toolbar, 0, wxEXPAND | wxALL, 5);

  auto *formula_sizer = new wxBoxSizer(wxHORIZONTAL);

  _cmb_sheet_selection = new wxComboBox(this, ID_SHEET_SELECTION_CMB);
  UpdateSheetCombo();
  formula_sizer->Add(_cmb_sheet_selection, 0, wxEXPAND | wxALL, 2);

  _btn_formula_selection = new wxButton(this, wxID_ANY, wxT("f(x)"));
  formula_sizer->Add(_btn_formula_selection, 0, wxEXPAND | wxALL, 2);
  // Textctrl for formula editing (normal text control for now)
  // TODO Replace by e.g. a syntax highlighting supporting control
  formula_sizer->Add(_text_control_formula, 1, wxEXPAND | wxALL, 2);

  sizer->Add(formula_sizer, 0, wxEXPAND | wxALL, 5);
  sizer->Add(_table_control, 1, wxEXPAND | wxALL, 5);

  SetSizerAndFit(sizer);

  _table_control->SetFocus();
}

void KalkulatorMainFrame::BindEvents() {
  Bind(wxEVT_MENU, &KalkulatorMainFrame::OnAbout, this, wxID_ABOUT);
  Bind(wxEVT_MENU, &KalkulatorMainFrame::OnExit, this, wxID_EXIT);
  Bind(wxEVT_MENU, &KalkulatorMainFrame::OnOpen, this, ID_Open);
  Bind(wxEVT_MENU, &KalkulatorMainFrame::OnNew, this, ID_New);
  Bind(wxEVT_MENU, &KalkulatorMainFrame::OnSave, this, ID_Save);
  Bind(wxEVT_MENU, &KalkulatorMainFrame::OnSaveAs, this, ID_SaveAs);
  Bind(wxEVT_MENU, &KalkulatorMainFrame::OnResizeColumn, this, ID_ResizeColumn);
  Bind(wxEVT_MENU, &KalkulatorMainFrame::OnResizeRow, this, ID_ResizeRow);
  Bind(wxEVT_MENU, &KalkulatorMainFrame::OnGotoCell, this, ID_GotoCell);
  Bind(wxEVT_MENU, &KalkulatorMainFrame::OnFormatCell, this, ID_FormatCell);
  Bind(wxEVT_MENU, &KalkulatorMainFrame::OnAddSheet, this, ID_AddSheet);
  Bind(wxEVT_MENU, &KalkulatorMainFrame::OnRemoveSheet, this, ID_RemoveSheet);
  Bind(wxEVT_MENU, &KalkulatorMainFrame::OnSearch, this, ID_Search);

  Bind(wxEVT_RIGHT_DOWN, &KalkulatorMainFrame::OnRightDown, this);
  Bind(wxEVT_CLOSE_WINDOW, &KalkulatorMainFrame::OnClose, this);

  Bind(wxEVT_COMBOBOX, &KalkulatorMainFrame::OnSheetSelectionCombo, this,
       ID_SHEET_SELECTION_CMB);
}

void KalkulatorMainFrame::OnRightDown(wxMouseEvent &WXUNUSED(event)) {
  // int x = event.GetPosition().x;
  // int y = event.GetPosition().y;

  // TODO
}

void KalkulatorMainFrame::OnExit(wxCommandEvent &WXUNUSED(event)) {
  Close(true);
}

void KalkulatorMainFrame::OnAbout(wxCommandEvent &WXUNUSED(event)) {
  wxAboutDialogInfo aboutInfo;
  aboutInfo.SetName("Kalkulator");
  aboutInfo.SetVersion(VERSION);
  aboutInfo.SetDescription(
      _("This is a simple and small spreadsheet application which uses a "
        "Lisp-like language for cell formulas.\n\nPlease see credits for "
        "information about the external libraries and projects used in this "
        "application."));
  aboutInfo.SetCopyright("(C) 2023 pderichs");
  aboutInfo.AddDeveloper("pderichs");
  aboutInfo.AddDeveloper("");
  aboutInfo.AddDeveloper(
      "Kalkulator uses some awesome external libraries and projects:");
  aboutInfo.AddDeveloper("wxWidgets https://www.wxwidgets.org/");
  aboutInfo.AddDeveloper("SQLite https://www.sqlite.org/index.html");
  aboutInfo.AddDeveloper("Google Material Design Icons "
                         "https://github.com/google/material-design-icons");
  wxAboutBox(aboutInfo);
}

void KalkulatorMainFrame::OnClose(wxCloseEvent &event) {
  if (!PermitLoseChanges()) {
    event.Veto();
    return;
  }

  event.Skip();
}

bool KalkulatorMainFrame::PermitLoseChanges() {
  if (_document->changed()) {
    if (wxMessageBox(
        wxT("Current content has not been saved. Your changes will "
            "be lost. Proceed?"),
        wxT("Please confirm"), wxICON_QUESTION | wxYES_NO, this) == wxNO)
      return false;
  }

  return true;
}

void KalkulatorMainFrame::OnNew(wxCommandEvent &WXUNUSED(event)) {
  if (!PermitLoseChanges()) {
    return;
  }

  _document->clear_and_initialize();

  Refresh();
}

void KalkulatorMainFrame::OnOpen(wxCommandEvent &WXUNUSED(event)) {
  // https://docs.wxwidgets.org/3.0/classwx_file_dialog.html

  if (!PermitLoseChanges()) {
    return;
  }

  wxString startFolder;
  if (!_document->file_path().empty()) {
    startFolder = wxPathOnly(_document->file_path());
  }

  wxFileDialog openFileDialog(this, _("Open Kalkulator file"), startFolder, "",
                              "Kalkulator files (*.kal)|*.kal",
                              wxFD_OPEN | wxFD_FILE_MUST_EXIST);
  if (openFileDialog.ShowModal() == wxID_CANCEL) {
    return;
  }

  TableWorkbookFile file;

  try {
    std::string file_path((const char *)openFileDialog.GetPath());
    file.open(file_path);
    _document->set_file_path("");
    file.read(_document);
    _document->clear_changed_flag();
    _document->set_file_path(file_path);
  } catch (TableWorkbookFileError &twfe) {
    wxLogError("Cannot open file '%s'.\n\nMessage: %s",
               openFileDialog.GetPath(), twfe.what());
  }

  Refresh();
}

void KalkulatorMainFrame::SaveDocument(const std::string &file_path) {
  TableWorkbookFile file;

  try {
    file.open(file_path);
    file.write(_document);
    _document->clear_changed_flag();
    _document->set_file_path(file.file_path());
  } catch (TableWorkbookFileError &twfe) {
    wxMessageBox(twfe.what(), wxT("Error"), wxICON_EXCLAMATION);
  }
}

void KalkulatorMainFrame::OnSaveAs(wxCommandEvent &WXUNUSED(event)) {
  wxString startFolder;

  if (!_document->file_path().empty()) {
    startFolder = wxPathOnly(_document->file_path());
  }

  wxFileDialog saveFileDialog(this, _("Save Kalkulator file"), startFolder, "",
                              "Kalkulator files (*.kal)|*.kal",
                              wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
  if (saveFileDialog.ShowModal() == wxID_CANCEL) {
    return;
  }

  SaveDocument((const char *)saveFileDialog.GetPath());
}

void KalkulatorMainFrame::UpdateFormulaBySelectedCell(
    const Location &location) {
  auto cell = _document->get_current_cell();

  if (cell && cell->row() == location.y() && cell->col() == location.x()) {
    std::string formula = cell->get_formula_content();
    _text_control_formula->SetValue(formula);
  } else {
    _text_control_formula->SetValue("");
  }
}

void KalkulatorMainFrame::send_event(TableEvent event_id, std::any param) {
  // wxPrintf("* EVENT: ");

  std::string new_content;

  switch (event_id) {
  case FORMULA_UPDATE:
    wxPrintf("FORMULA UPDATE\n");

    _table_control->SetFocus();

    try {
      new_content = std::any_cast<std::string>(param);

      wxPrintf("  Formula update content: %s\n", new_content);

      // Apply new content to cell
      _document->update_content_current_cell(new_content);
    } catch (const std::bad_any_cast &e) {
      wxPrintf("*** EVENT: bad any cast for formula update. Event will be "
               "ignored.\n");
    } catch (const std::runtime_error &rte) {
      wxPrintf("*** CAUGHT RUNTIME EXCEPTION: %s\n", rte.what());
      wxMessageBox(
          wxT("Cell content could not be updated - Please verify your input."),
          wxT("Error"), wxICON_EXCLAMATION);
    }

    break;

  case FORMULA_CANCEL: {
    if (_text_control_formula->IsModified() &&
        wxMessageBox(
            wxT("Current content has not been applied. Your changes will "
                "be lost. Proceed?"),
            wxT("Please confirm"), wxICON_QUESTION | wxYES_NO, this) == wxYES) {
      _table_control->SetFocus();

      std::optional<Location> location =
          _document->current_sheet_selected_cell();
      if (location) {
        UpdateFormulaBySelectedCell(*location);
      }
    }

    break;
  }

  case CELL_UPDATED:
    try {
      auto location(std::any_cast<Location>(param));

      _table_control->OnCellUpdate(location);

      UpdateFormulaBySelectedCell(location);
    } catch (const std::bad_any_cast &e) {
      wxPrintf("*** EVENT: bad any cast for cell update. Event will be "
               "ignored.\n");
    }

    break;

  case CURRENT_CELL_LOCATION_UPDATED:
    try {
      auto location(std::any_cast<Location>(param));

      auto cell = _document->get_cell(location);

      if (cell) {
        // Update formula text
        std::string formula = cell->get_formula_content();
        _text_control_formula->SetValue(formula);

        _table_control->SetCurrentCell(cell->location());
        _table_control->Refresh();
      }

    } catch (const std::bad_any_cast &e) {
      wxPrintf("*** EVENT: bad any cast for cell update. Event will be "
               "ignored.\n");
    }

    break;

  case CELL_VIEW_SCROLL_EVENT: {
    auto scroll_pos = std::any_cast<Location>(param);
    _table_control->update_scroll_positions(scroll_pos);
    break;
  }

  case SHEET_SELECTION_UPDATED: {
    auto sheet = _document->current_sheet();
    _cmb_sheet_selection->SetValue(sheet->name());
    _table_control->Refresh();
    break;
  }

  case ROW_HEIGHT_UPDATED:
  case COLUMN_WIDTH_UPDATED:
    _table_control->Refresh();
    break;

  case HEADER_GOT_FOCUS:
    _table_control->SetFocus();
    break;

  case NAVIGATE_SEARCH_RESULT_ITEM: {
    auto search_result = std::any_cast<TableSearchResultItem>(param);
    _document->select_sheet(search_result.sheet);
    _document->select_cell(search_result.location);
    _table_control->ScrollToCurrentCell(CELL_WINDOW_LOCATION_CENTER);
    break;
  }

  case MESSAGE_BOX: {
    std::string msg = std::any_cast<std::string>(param);
    wxMessageBox(msg);
    break;
  }

  case EDIT_CELL: {
    _text_control_formula->SetFocus();
    break;
  }
  }
}

void KalkulatorMainFrame::OnSave(wxCommandEvent &event) {
  if (_document->file_path().empty()) {
    OnSaveAs(event);
  } else {
    SaveDocument(_document->file_path());
  }
}

void KalkulatorMainFrame::UpdateSheetCombo() {
  _cmb_sheet_selection->Clear();

  int n = 0;
  int current = -1;
  for (const auto &sheet : _document->sheets()) {
    _cmb_sheet_selection->Append(sheet->name());

    if (sheet == _document->current_sheet()) {
      current = n;
    }

    n++;
  }

  if (current >= 0) {
    _cmb_sheet_selection->SetSelection(current);
  }
}

void KalkulatorMainFrame::OnSheetSelectionCombo(
    wxCommandEvent &WXUNUSED(event)) {
  wxString sheet_name = _cmb_sheet_selection->GetValue();
  _document->select_sheet_by_name(static_cast<const char *>(sheet_name));
}

void KalkulatorMainFrame::OnResizeColumn(wxCommandEvent &WXUNUSED(event)) {
  size_t current_width = _document->get_current_column_width();

  long entry = wxGetNumberFromUser("Resize column.", "Width:", "Resize column",
                                   current_width, 20, 1000);

  if (entry < 0) {
    return;
  }

  _document->set_current_column_width(static_cast<size_t>(entry));
}

void KalkulatorMainFrame::OnResizeRow(wxCommandEvent &WXUNUSED(event)) {
  size_t current_height = _document->get_current_row_height();

  long entry = wxGetNumberFromUser("Resize row.", "Height:", "Resize row",
                                   current_height, 20, 1000);

  if (entry < 0) {
    return;
  }

  _document->set_current_row_height(static_cast<size_t>(entry));
}

void KalkulatorMainFrame::OnGotoCell(wxCommandEvent &WXUNUSED(event)) {
  wxString raw_input =
      wxGetTextFromUser(wxT("Please enter row and col:"), wxT("Goto cell"), "");

  if (raw_input.IsEmpty()) {
    return;
  }

  int row;
  int col;

  std::string input{(const char *)raw_input};

  std::regex exp{"(\\d+) (\\d+)"};
  std::smatch sm;

  if (!std::regex_search(input, sm, exp)) {
    wxMessageBox("Invalid input. Please enter the coordinates row and col "
                 "separated by space.",
                 "Goto cell error", wxICON_EXCLAMATION);

    return;
  }

  row = std::stoi(sm[1].str());
  col = std::stoi(sm[2].str());

  _document->select_cell(Location(col, row));

  _table_control->ScrollToCurrentCell(CELL_WINDOW_LOCATION_CENTER);
}

void KalkulatorMainFrame::OnFormatCell(wxCommandEvent &WXUNUSED(event)) {
  // TODO Set font / format options of current cell in dialog

  TableCellFormatDlg format_dialog(this, _document->get_current_cell_format(),
                                   _table_control->GetCellViewFont());

  if (format_dialog.ShowModal() == wxID_OK) {
    wxPrintf("OK\n");

    TableCellFormat format = format_dialog.GetFormat();

    _document->set_current_cell_format(format);

    Refresh();
  }
}

void KalkulatorMainFrame::OnAddSheet(wxCommandEvent &WXUNUSED(event)) {
  wxString raw_input =
      wxGetTextFromUser(wxT("Sheet Name:"), wxT("Add sheet"), "");

  if (raw_input.IsEmpty()) {
    return;
  }

  _document->add_sheet(static_cast<const char *>(raw_input));
  UpdateSheetCombo();
}

void KalkulatorMainFrame::OnRemoveSheet(wxCommandEvent &WXUNUSED(event)) {
  _document->remove_current_sheet();
}

void KalkulatorMainFrame::OnSearch(wxCommandEvent &WXUNUSED(event)) {
  wxString raw_input =
      wxGetTextFromUser(wxT("Search term:"), wxT("Search"), _last_search_term);

  _last_search_term = raw_input;

  if (raw_input.IsEmpty()) {
    return;
  }

  TableSearchResult search_result =
      _document->search_sheets(static_cast<const char *>(raw_input));

  if (search_result.empty()) {
    wxMessageBox("No occurrences found.", "Search");
    return;
  }

  if (!_search_results_dlg) {
    _search_results_dlg = new TableSearchResultsDlg(this, this, wxID_ANY);
  }

  _search_results_dlg->Initialize(search_result);
  _search_results_dlg->Show();
}
