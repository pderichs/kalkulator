#include <memory>
#include <sched.h>
#include <stdexcept>
#include <tuple>
#include <wx/dcclient.h>
#include <wx/event.h>
#include <wx/msgdlg.h>
#include <wx/rawbmp.h>
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
#include "view/table_control.h"
#include "view/table_formula_text_control.h"

#if !defined(WX_PRECOMP)
#include <wx/wx.h>
#endif

#define WIDTH 1024
#define HEIGHT 768

class MyApp : public wxApp {
public:
  virtual bool OnInit();
};

class KalkulatorMainFrame : public wxFrame, public EventSink {
public:
  KalkulatorMainFrame();
  virtual ~KalkulatorMainFrame() = default;

  virtual void OnClose(wxCloseEvent &event);

private:
  void OnExit(wxCommandEvent &event);
  void OnAbout(wxCommandEvent &event);
  void OnOpen(wxCommandEvent &event);
  void OnNew(wxCommandEvent &event);
  void OnSaveAs(wxCommandEvent &event);
  void OnRightDown(wxMouseEvent &event);
  void OnKeyPress(wxKeyEvent &event);

  void BindEvents();

  void SetupUserInterface();
  virtual void send_event(TableEvent event_id, std::any param);

  bool PermitLoseChanges();

private:
  TableWorkbookDocument _document;
  TableControl *_table_control;
  wxTextCtrl *_text_control_formula;
  LispExecutionContext _execution_context;
};

enum {
  ID_Open = 1,
  ID_SaveAs,
  ID_New,
};

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
    : wxFrame(NULL, wxID_ANY, "Kalkulator 0.0.1a"), _document(this) {
  ValueConverter::update_execution_context(&_execution_context);

  _execution_context.add_function(
      "cell", std::make_shared<LispExecutionContextCellReference>(&_document));
  _execution_context.add_function(
      "cell_range",
      std::make_shared<LispExecutionContextCellRange>(&_document));

  wxMenu *menuFile = new wxMenu();
  menuFile->Append(ID_New, "&New\tCtrl-N",
                   "Creates a new spreadsheet workbook");
  menuFile->Append(ID_Open, "&Open...\tCtrl-O", "Opens a Kalkulator file");
  menuFile->Append(ID_SaveAs, "&Save as...\tCtrl-S",
                   "Saves the current workbook");
  menuFile->AppendSeparator();
  menuFile->Append(wxID_EXIT);

  wxMenu *menuHelp = new wxMenu();
  menuHelp->Append(wxID_ABOUT);

  wxMenuBar *menuBar = new wxMenuBar();
  menuBar->Append(menuFile, "&File");
  menuBar->Append(menuHelp, "&Help");

  _table_control = new TableControl(&_document, this, this, wxID_ANY,
                                    wxDefaultPosition, wxDefaultSize);

  _text_control_formula = new TableFormulaTextControl(
      this, this, -1, "", wxDefaultPosition, wxDefaultSize);

  SetMenuBar(menuBar);

  CreateStatusBar();
  SetStatusText("Welcome to Kalkulator!");

  BindEvents();

  SetupUserInterface();
}

void KalkulatorMainFrame::SetupUserInterface() {
  wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

  // Textctrl for formula editing (normal text control for now)
  // TODO Replace by e.g. a syntax highlighting supporting control
  sizer->Add(_text_control_formula, 0, wxEXPAND | wxALL, 5);

  // Table
  sizer->Add(_table_control, 1, wxEXPAND | wxALL, 5);

  SetSizerAndFit(sizer);

  _table_control->SetFocus();
}

void KalkulatorMainFrame::BindEvents() {
  Bind(wxEVT_MENU, &KalkulatorMainFrame::OnAbout, this, wxID_ABOUT);
  Bind(wxEVT_MENU, &KalkulatorMainFrame::OnExit, this, wxID_EXIT);
  Bind(wxEVT_MENU, &KalkulatorMainFrame::OnOpen, this, ID_Open);
  Bind(wxEVT_MENU, &KalkulatorMainFrame::OnNew, this, ID_New);
  Bind(wxEVT_MENU, &KalkulatorMainFrame::OnSaveAs, this, ID_SaveAs);

  Bind(wxEVT_RIGHT_DOWN, &KalkulatorMainFrame::OnRightDown, this);
  Bind(wxEVT_CLOSE_WINDOW, &KalkulatorMainFrame::OnClose, this);
  Bind(wxEVT_CHAR_HOOK, &KalkulatorMainFrame::OnKeyPress, this);
}

void KalkulatorMainFrame::OnRightDown(wxMouseEvent &WXUNUSED(event)) {
  // int x = event.GetPosition().x;
  // int y = event.GetPosition().y;

  // KFigure *p = _document.getFigureAtPosition(x, y);
  // if (p) {
  //   wxMenu figureMenu;
  //   figureMenu.Append(ID_FIGURE_PROPERTIES, wxT("&Properties..."),
  //                     wxT("Properties"));
  //   PopupMenu(&figureMenu);
  // }
}

void KalkulatorMainFrame::OnExit(wxCommandEvent &WXUNUSED(event)) {
  Close(true);
}

void KalkulatorMainFrame::OnAbout(wxCommandEvent &WXUNUSED(event)) {
  wxMessageBox("This is a sample!", "About", wxOK | wxICON_INFORMATION);
}

void KalkulatorMainFrame::OnClose(wxCloseEvent &event) {
  if (!PermitLoseChanges()) {
    event.Veto();
    return;
  }

  event.Skip();
}

bool KalkulatorMainFrame::PermitLoseChanges() {
  if (_document.changed()) {
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

  _document.clear_and_initialize();

  Refresh();
}

void KalkulatorMainFrame::OnOpen(wxCommandEvent &WXUNUSED(event)) {
  // https://docs.wxwidgets.org/3.0/classwx_file_dialog.html

  if (!PermitLoseChanges()) {
    return;
  }

  wxString startFolder;
  if (!_document.file_path().empty()) {
    startFolder = wxPathOnly(_document.file_path());
  }

  wxFileDialog openFileDialog(this, _("Open Kalkulator file"), startFolder, "",
                              "Kalkulator files (*.kal)|*.kal",
                              wxFD_OPEN | wxFD_FILE_MUST_EXIST);
  if (openFileDialog.ShowModal() == wxID_CANCEL) {
    return;
  }

  TableWorkbookFile file;

  try {
    file.open((const char *)openFileDialog.GetPath());
    file.read(_document);
  } catch (TableWorkbookFileError &twfe) {
    wxLogError("Cannot open file '%s'.", openFileDialog.GetPath());
  }

  Refresh();
}

void KalkulatorMainFrame::OnSaveAs(wxCommandEvent &WXUNUSED(event)) {
  wxString startFolder;

  if (!_document.file_path().empty()) {
    startFolder = wxPathOnly(_document.file_path());
  }

  wxFileDialog saveFileDialog(this, _("Save Kalkulator file"), startFolder, "",
                              "Kalkulator files (*.kal)|*.kal",
                              wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
  if (saveFileDialog.ShowModal() == wxID_CANCEL) {
    return;
  }

  TableWorkbookFile file;

  try {
    file.open((const char *)saveFileDialog.GetPath());
    file.write(_document);
    _document.clear_changed_flag();
  } catch (TableWorkbookFileError &twfe) {
    wxMessageBox(twfe.what(), wxT("Error"), wxICON_EXCLAMATION);
  }
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
      _document.update_content_current_cell(new_content);
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

  case CELL_UPDATED:
    try {
      Location location(std::any_cast<Location>(param));

      _table_control->OnCellUpdate(location);

      auto cell = _document.get_current_cell();

      if (cell && cell->row() == location.y() && cell->col() == location.x()) {
        std::string formula = cell->get_formula_content();
        _text_control_formula->SetValue(formula);
      } else {
        _text_control_formula->SetValue("");
      }
    } catch (const std::bad_any_cast &e) {
      wxPrintf("*** EVENT: bad any cast for cell update. Event will be "
               "ignored.\n");
    }

    break;

  case CURRENT_CELL_LOCATION_UPDATED:
    try {
      Location location(std::any_cast<Location>(param));

      auto cell = _document.get_cell(location);

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

  case CELL_VIEW_SCROLL_EVENT:
    Location scroll_pos = std::any_cast<Location>(param);
    _table_control->update_scroll_positions(scroll_pos);
    break;
  }
}
