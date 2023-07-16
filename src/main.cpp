#include <wx/dcclient.h>
#include <wx/event.h>
#include <wx/rawbmp.h>
#include <wx/timer.h>
#include <wx/wxprec.h>

#include <cstdlib>
#include <iostream>

#include "model/event_sink.h"
#include "model/table/table_workbook_document.h"
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

class MyFrame : public wxFrame, public EventSink {
public:
  MyFrame();
  virtual ~MyFrame() = default;

  virtual void OnClose(wxCloseEvent &event);

private:
  void OnExit(wxCommandEvent &event);
  void OnAbout(wxCommandEvent &event);
  void OnOpen(wxCommandEvent &event);
  void OnSaveAs(wxCommandEvent &event);
  void OnRightDown(wxMouseEvent &event);
  void OnKeyPress(wxKeyEvent &event);

  void BindEvents();

  void SetupUserInterface();
  virtual void send_event(TableEvent event_id, std::any param);

private:
  TableWorkbookDocument _document;
  TableControl *_table_control;
  wxTextCtrl *_text_control_formula;
};

enum {
  ID_Open = 1,
  ID_SaveAs,
};

wxIMPLEMENT_APP(MyApp);

/// -------------------------------------------------------------
/// Implementation
///
///

bool MyApp::OnInit() {
  MyFrame *frame = new MyFrame();
  frame->SetSize(WIDTH, HEIGHT);
  frame->Show();
  return true;
}

MyFrame::MyFrame()
    : wxFrame(NULL, wxID_ANY, "Kalkulator 0.1a"), _document(this) {
  wxMenu *menuFile = new wxMenu();
  menuFile->Append(ID_Open, "&Open...\tCtrl-O", "Opens a figures file");
  menuFile->Append(ID_SaveAs, "&Save as...\tCtrl-S",
                   "Saves the current figures to a file");
  menuFile->AppendSeparator();
  menuFile->Append(wxID_EXIT);

  wxMenu *menuHelp = new wxMenu();
  menuHelp->Append(wxID_ABOUT);

  wxMenuBar *menuBar = new wxMenuBar();
  menuBar->Append(menuFile, "&File");
  menuBar->Append(menuHelp, "&Help");

  _table_control =
      new TableControl(&_document, this, this, wxID_ANY, wxDefaultPosition,
                       wxDefaultSize, wxWANTS_CHARS);

  _text_control_formula = new TableFormulaTextControl(
      this, this, -1, "", wxDefaultPosition, wxDefaultSize);

  SetMenuBar(menuBar);

  CreateStatusBar();
  SetStatusText("Welcome to Kalkulator!");

  BindEvents();

  SetupUserInterface();
}

void MyFrame::SetupUserInterface() {
  wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

  // Textctrl for formula editing (normal text control for now)
  // TODO Replace by e.g. a syntax highlighting supporting control
  sizer->Add(_text_control_formula, 0, wxEXPAND | wxALL, 5);

  // Table
  sizer->Add(_table_control, 1, wxEXPAND | wxALL, 5);

  SetSizerAndFit(sizer);

  _table_control->SetFocus();
}

void MyFrame::BindEvents() {
  Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
  Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
  Bind(wxEVT_MENU, &MyFrame::OnOpen, this, ID_Open);
  Bind(wxEVT_MENU, &MyFrame::OnSaveAs, this, ID_SaveAs);

  Bind(wxEVT_RIGHT_DOWN, &MyFrame::OnRightDown, this);
  Bind(wxEVT_CLOSE_WINDOW, &MyFrame::OnClose, this);
  Bind(wxEVT_CHAR_HOOK, &MyFrame::OnKeyPress, this);
}

void MyFrame::OnRightDown(wxMouseEvent &event) {
  int x = event.GetPosition().x;
  int y = event.GetPosition().y;

  // KFigure *p = _document.getFigureAtPosition(x, y);
  // if (p) {
  //   wxMenu figureMenu;
  //   figureMenu.Append(ID_FIGURE_PROPERTIES, wxT("&Properties..."),
  //                     wxT("Properties"));
  //   PopupMenu(&figureMenu);
  // }
}

void MyFrame::OnExit(wxCommandEvent &event) { Close(true); }

void MyFrame::OnAbout(wxCommandEvent &event) {
  wxMessageBox("This is a sample!", "About", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnClose(wxCloseEvent &event) {
  // if (_document.changed()) {
  //   if (wxMessageBox(_("Current content has not been saved! Proceed?"),
  //                    _("Please confirm Exiting"), wxICON_QUESTION | wxYES_NO,
  //                    this) == wxNO) {
  //     event.Veto();
  //     return;
  //   }
  // }

  event.Skip();
}

void MyFrame::OnOpen(wxCommandEvent &event) {
  // https://docs.wxwidgets.org/3.0/classwx_file_dialog.html

  // if (_document.changed()) {
  //   if (wxMessageBox(_("Current content has not been saved! Proceed?"),
  //                    _("Please confirm"), wxICON_QUESTION | wxYES_NO,
  //                    this) == wxNO)
  //     return;
  // }

  // wxString startFolder;
  // if (!_document.currentFile().empty()) {
  //   startFolder = wxPathOnly(_document.currentFile());
  // }

  // wxFileDialog openFileDialog(this, _("Open Figures file"), startFolder, "",
  //                             "Figures files (*.fig)|*.fig",
  //                             wxFD_OPEN | wxFD_FILE_MUST_EXIST);
  // if (openFileDialog.ShowModal() == wxID_CANCEL) {
  //   return;
  // }

  // if (!_document.open(openFileDialog.GetPath(), _canvas)) {
  //   wxLogError("Cannot open file '%s'.", openFileDialog.GetPath());
  // }

  Refresh();
}

void MyFrame::OnSaveAs(wxCommandEvent &event) {
  // wxString startFolder;
  // if (!_document.currentFile().empty()) {
  //   startFolder = wxPathOnly(_document.currentFile());
  // }

  // wxFileDialog saveFileDialog(this, _("Save Figures file"), startFolder, "",
  //                             "Figures files (*.fig)|*.fig",
  //                             wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
  // if (saveFileDialog.ShowModal() == wxID_CANCEL) {
  //   return;
  // }

  // if (!_document.save(saveFileDialog.GetPath())) {
  //   wxLogError("Cannot save current contents in file '%s'.",
  //              saveFileDialog.GetPath());
  // }
}

void MyFrame::OnKeyPress(wxKeyEvent &event) {
  // Handle the keypress event here
  int keyCode = event.GetKeyCode();

  // Example: Print the keycode to the console
  wxPrintf("MyFrame: Key pressed: %d\n", keyCode);

  switch (keyCode) {
  case WXK_F2:
    _text_control_formula->SetFocus();
    break;
  }

  event.Skip();
}

void MyFrame::send_event(TableEvent event_id, std::any param) {
  wxPrintf("* EVENT: ");

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
    }

    break;

  case CELL_UPDATED:
    try {
      Location location(std::any_cast<Location>(param));

      _table_control->OnCellUpdate(location);
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
      }

    } catch (const std::bad_any_cast &e) {
      wxPrintf("*** EVENT: bad any cast for cell update. Event will be "
               "ignored.\n");
    }

    break;
  }

  // TODO
}
