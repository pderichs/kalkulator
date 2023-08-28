#include <memory>
#include <regex>
#include <sched.h>
#include <sstream>
#include <stdexcept>
#include <tuple>
#include <wx/aboutdlg.h>
#include <wx/dcclient.h>
#include <wx/event.h>
#include <wx/filename.h>
#include <wx/msgdlg.h>
#include <wx/numdlg.h>
#include <wx/rawbmp.h>
#include <wx/stdpaths.h>
#include <wx/timer.h>
#include <wx/wxprec.h>

#include <cstdlib>
#include <iostream>

#include "model/event_sink.h"
#include "model/lisp/lisp_execution_context.h"
#include "model/lisp/value_converter.h"
#include "model/table/lisp_execution_context_cell_range.h"
#include "model/table/lisp_execution_context_cell_reference.h"
#include "model/table/table_workbook_document.h"
#include "model/table/table_workbook_file.h"
#include "model/table/table_workbook_file_error.h"
#include "tests.h"
#include "view/kalkulator_system_colors.h"
#include "view/table_control.h"
#include "view/table_formula_text_control.h"

#if !defined(WX_PRECOMP)
#include <wx/wx.h>
#endif

#define VERSION "0.0.1a"

#define WIDTH 1024
#define HEIGHT 768

typedef std::pair<std::string, std::string> IconPaths;

// clang-format off
// Map between icon_key and dark mode and bright mode icon paths.
std::map<std::string, IconPaths> IconDictionary = {
    {"new", {"outline_insert_drive_file_white_18dp.png", "outline_insert_drive_file_black_18dp.png"}},
    {"open", {"outline_folder_open_white_18dp.png", "outline_folder_open_black_18dp.png"}},
    {"save", {"outline_save_white_18dp.png", "outline_save_black_18dp.png"}},
    {"height", {"outline_height_white_18dp.png", "outline_height_black_18dp.png"}},
};
// clang-format on

class MyApp : public wxApp {
public:
  virtual bool OnInit();
};

class KalkulatorMainFrame : public wxFrame, public EventSink {
public:
  KalkulatorMainFrame();
  virtual ~KalkulatorMainFrame();

  virtual void OnClose(wxCloseEvent &event);

private:
  void OnExit(wxCommandEvent &event);
  void OnAbout(wxCommandEvent &event);
  void OnOpen(wxCommandEvent &event);
  void OnNew(wxCommandEvent &event);
  void OnSave(wxCommandEvent &event);
  void OnSaveAs(wxCommandEvent &event);
  void OnRightDown(wxMouseEvent &event);
  void OnKeyPress(wxKeyEvent &event);
  void OnSheetSelectionCombo(wxCommandEvent &event);
  void OnResizeColumn(wxCommandEvent &event);
  void OnResizeRow(wxCommandEvent &event);
  void OnGotoCell(wxCommandEvent &WXUNUSED(event));

  void BindEvents();
  void CreateToolbar();

  void InitializeModel();
  void InitializeMenu();
  void InitializeIcons();

  void UpdateSheetCombo();
  void SetupUserInterface();
  virtual void send_event(TableEvent event_id, std::any param);

  bool PermitLoseChanges();

  bool IsDarkUI() const {
    wxSystemAppearance s = wxSystemSettings::GetAppearance();
    return s.IsDark();
  }

  wxString GetIconPath(const std::string &icon_id) const;

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
};

enum {
  ID_Open = 1,
  ID_Save,
  ID_SaveAs,
  ID_New,
  ID_ResizeColumn,
  ID_ResizeRow,
  ID_GotoCell,
};

enum { ID_SHEET_SELECTION_CMB = wxID_HIGHEST + 1 };

wxIMPLEMENT_APP(MyApp);

/// -------------------------------------------------------------
/// Implementation
///
///

bool MyApp::OnInit() {
  if (argc > 1) {
    if (argv[1] == "unittests") {
      wxPrintf("Running tests...\n");

      if (run_tests() != 0) {
        wxPrintf("TESTS FAILED!\n");
      } else {
        wxPrintf("TESTS OK.\n");
      }

      exit(0);
    }
  }

  KalkulatorMainFrame *frame = new KalkulatorMainFrame();
  frame->SetSize(WIDTH, HEIGHT);
  frame->Show();

  return true;
}

KalkulatorMainFrame::KalkulatorMainFrame()
    : wxFrame(NULL, wxID_ANY, "Kalkulator " VERSION) {
  _document = std::make_shared<TableWorkbookDocument>(this);

  InitializeModel();

  InitializeIcons();

  InitializeMenu();

  CreateStatusBar();
  SetStatusText("Welcome to Kalkulator!");

  BindEvents();

  CreateToolbar();

  SetupUserInterface();
}

KalkulatorMainFrame::~KalkulatorMainFrame() {
  if (_icon_new) {
    delete _icon_new;
    _icon_new = NULL;
  }

  if (_icon_height) {
    delete _icon_height;
    _icon_height = NULL;
  }

  if (_icon_width) {
    delete _icon_width;
    _icon_width = NULL;
  }

  if (_icon_open) {
    delete _icon_open;
    _icon_open = NULL;
  }

  if (_icon_save) {
    delete _icon_save;
    _icon_save = NULL;
  }

  if (_toolbar) {
    delete _toolbar;
    _toolbar = NULL;
  }

  if (_table_control) {
    delete _table_control;
    _table_control = NULL;
  }

  if (_text_control_formula) {
    delete _text_control_formula;
    _text_control_formula = NULL;
  }
}

void KalkulatorMainFrame::InitializeIcons() {
  _icon_new = new wxBitmap(GetIconPath("new"), wxBITMAP_TYPE_PNG);
  _icon_open = new wxBitmap(GetIconPath("open"), wxBITMAP_TYPE_PNG);
  _icon_save = new wxBitmap(GetIconPath("save"), wxBITMAP_TYPE_PNG);
  _icon_height = new wxBitmap(GetIconPath("height"), wxBITMAP_TYPE_PNG);

  wxBitmap temp(GetIconPath("height"), wxBITMAP_TYPE_PNG);
  wxImage image = temp.ConvertToImage();
  image = image.Rotate90();
  _icon_width = new wxBitmap(image);
}

void KalkulatorMainFrame::InitializeModel() {
  ValueConverter::set_execution_context(&_execution_context);

  _execution_context.add_function(
      "cell", std::make_shared<LispExecutionContextCellReference>(_document));
  _execution_context.add_function(
      "cell_range", std::make_shared<LispExecutionContextCellRange>(_document));

  _sys_colors = std::make_shared<KalkulatorSystemColors>();

  _table_control = new TableControl(_sys_colors, _document, this, this,
                                    wxID_ANY, wxDefaultPosition, wxDefaultSize);

  _text_control_formula = new TableFormulaTextControl(
      this, this, -1, "", wxDefaultPosition, wxDefaultSize);
}

void KalkulatorMainFrame::InitializeMenu() {
  wxMenuItem *item;

  wxMenu *menuFile = new wxMenu();
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

  wxMenu *menuTable = new wxMenu();
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

  wxMenu *menuHelp = new wxMenu();
  menuHelp->Append(wxID_ABOUT);

  wxMenuBar *menuBar = new wxMenuBar();
  menuBar->Append(menuFile, "&File");
  menuBar->Append(menuTable, "&Table");
  menuBar->Append(menuHelp, "&Help");
  SetMenuBar(menuBar);
}

wxString KalkulatorMainFrame::GetIconPath(const std::string &icon_key) const {
  auto it = IconDictionary.find(icon_key);
  if (it == IconDictionary.end()) {
    // TODO: Default icon path?
    return "not_existing";
  }

  const auto &paths = it->second;

  wxString path;
  if (IsDarkUI()) {
    path = paths.first.c_str();
  } else {
    path = paths.second.c_str();
  }

  wxFileName f(wxStandardPaths::Get().GetExecutablePath());
  f.SetFullName(path);
  return wxString(f.GetLongPath());
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
  wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

  sizer->Add(_toolbar, 0, wxEXPAND | wxALL, 5);

  wxBoxSizer *formula_sizer = new wxBoxSizer(wxHORIZONTAL);

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

  Bind(wxEVT_RIGHT_DOWN, &KalkulatorMainFrame::OnRightDown, this);
  Bind(wxEVT_CLOSE_WINDOW, &KalkulatorMainFrame::OnClose, this);
  Bind(wxEVT_CHAR_HOOK, &KalkulatorMainFrame::OnKeyPress, this);

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
      "Kalulator uses some awesome external libraries and projects:");
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
    file.read(_document);
    _document->clear_changed_flag();
    _document->set_file_path(file_path);
  } catch (TableWorkbookFileError &twfe) {
    wxLogError("Cannot open file '%s'.\n\nMessage: %s", openFileDialog.GetPath(), twfe.what());
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

void KalkulatorMainFrame::OnKeyPress(wxKeyEvent &event) {
  // Handle the keypress event here
  int keyCode = event.GetKeyCode();

  // wxPrintf("KalkulatorMainFrame: Key pressed: %d\n", keyCode);

  switch (keyCode) {
  case WXK_F2:
    _text_control_formula->SetFocus();
    break;
  }

  event.Skip();
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
  }

  break;

  case CELL_UPDATED:
    try {
      Location location(std::any_cast<Location>(param));

      _table_control->OnCellUpdate(location);

      UpdateFormulaBySelectedCell(location);
    } catch (const std::bad_any_cast &e) {
      wxPrintf("*** EVENT: bad any cast for cell update. Event will be "
               "ignored.\n");
    }

    break;

  case CURRENT_CELL_LOCATION_UPDATED:
    try {
      Location location(std::any_cast<Location>(param));

      auto cell = _document->get_cell(location);

      if (cell) {
        auto unwrapped_cell = *cell;

        // Update formula text
        std::string formula = unwrapped_cell->get_formula_content();
        _text_control_formula->SetValue(formula);

        _table_control->Refresh();
      }

    } catch (const std::bad_any_cast &e) {
      wxPrintf("*** EVENT: bad any cast for cell update. Event will be "
               "ignored.\n");
    }

    break;

  case CELL_VIEW_SCROLL_EVENT: {
    Location scroll_pos = std::any_cast<Location>(param);
    _table_control->update_scroll_positions(scroll_pos);
    break;
  }

  case SHEET_SELECTION_UPDATED:
  case ROW_HEIGHT_UPDATED:
  case COLUMN_WIDTH_UPDATED:
    _table_control->Refresh();
    break;

  case HEADER_GOT_FOCUS:
    _table_control->SetFocus();
    break;
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
    _cmb_sheet_selection->Append(sheet->name);

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

  _table_control->ScrollToCurrentCell(); // TODO Add option to scroll cell to
                                         // center of view?
}
