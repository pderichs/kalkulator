#include <wx/dcclient.h>
#include <wx/event.h>
#include <wx/rawbmp.h>
#include <wx/timer.h>
#include <wx/wxprec.h>

#include <cstdlib>
#include <iostream>

#if !defined(WX_PRECOMP)
#include <wx/wx.h>
#endif

#define WIDTH 1024
#define HEIGHT 768

class MyApp : public wxApp {
public:
  virtual bool OnInit();
};

class MyFrame : public wxFrame {
public:
  MyFrame();

  virtual void OnClose(wxCloseEvent &event);

private:
  void OnNewRect(wxCommandEvent &event);
  void OnNewConnector(wxCommandEvent &event);
  void OnDeleteFigure(wxCommandEvent &event);

  void OnExit(wxCommandEvent &event);
  void OnAbout(wxCommandEvent &event);
  void OnOpen(wxCommandEvent &event);
  void OnSaveAs(wxCommandEvent &event);
  void OnRightDown(wxMouseEvent &event);
  void OnFigureProperties(wxCommandEvent &event);

  void BindEvents();

private:
  // KRectsDocument _document;
  // KCanvas *_canvas;
};

enum {
  ID_NewRect = 1,
  ID_Open,
  ID_SaveAs,
  ID_NewConnector,
  ID_FIGURE_PROPERTIES,
  ID_DeleteFigure
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

MyFrame::MyFrame() : wxFrame(NULL, wxID_ANY, "Graph") {
  wxMenu *menuFile = new wxMenu();
  menuFile->Append(ID_Open, "&Open...\tCtrl-O", "Opens a figures file");
  menuFile->Append(ID_SaveAs, "&Save as...\tCtrl-S",
                   "Saves the current figures to a file");
  menuFile->AppendSeparator();
  menuFile->Append(wxID_EXIT);

  wxMenu *menuShapes = new wxMenu();
  menuShapes->Append(ID_NewRect, "&New Rect\tCtrl-R",
                     "Creates a new Rect on the canvas");
  menuShapes->Append(ID_NewConnector, "&New Connector\tCtrl-R",
                     "Creates a new Connector on the canvas");
  menuShapes->AppendSeparator();
  menuShapes->Append(ID_DeleteFigure, "Delete Figure\tDEL",
                     "Deletes the current figure");

  wxMenu *menuHelp = new wxMenu();
  menuHelp->Append(wxID_ABOUT);

  wxMenuBar *menuBar = new wxMenuBar();
  menuBar->Append(menuFile, "&File");
  menuBar->Append(menuShapes, "&Shapes");
  menuBar->Append(menuHelp, "&Help");

  // _canvas = new KCanvas(&_document, this, wxID_ANY, wxDefaultPosition,
  //                       wxDefaultSize, wxWANTS_CHARS);

  SetMenuBar(menuBar);

  CreateStatusBar();
  SetStatusText("Welcome to Kalkulator!");

  BindEvents();
}

void MyFrame::BindEvents() {
  Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
  Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
  Bind(wxEVT_MENU, &MyFrame::OnNewRect, this, ID_NewRect);
  Bind(wxEVT_MENU, &MyFrame::OnNewConnector, this, ID_NewConnector);
  Bind(wxEVT_MENU, &MyFrame::OnOpen, this, ID_Open);
  Bind(wxEVT_MENU, &MyFrame::OnSaveAs, this, ID_SaveAs);
  Bind(wxEVT_MENU, &MyFrame::OnFigureProperties, this, ID_FIGURE_PROPERTIES);
  Bind(wxEVT_MENU, &MyFrame::OnDeleteFigure, this, ID_DeleteFigure);

  Bind(wxEVT_RIGHT_DOWN, &MyFrame::OnRightDown, this);
  Bind(wxEVT_CLOSE_WINDOW, &MyFrame::OnClose, this);
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

void MyFrame::OnFigureProperties(wxCommandEvent &event) {
  // TODO: Eigenschaftsdialog
}

void MyFrame::OnExit(wxCommandEvent &event) { Close(true); }

void MyFrame::OnAbout(wxCommandEvent &event) {
  wxMessageBox("This is a sample!", "About", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnNewConnector(wxCommandEvent &event) {
  // KConnector *pConnector =
  //     (KConnector *)_document.createFigure(FIGURETYPE_CONNECTOR, _canvas);
  // pConnector->setStartX(10);
  // pConnector->setStartY(10);
  // pConnector->setEndX(10);
  // pConnector->setEndY(100);
  // pConnector->initializeAnfasserRects();
  // _document.addFigure(pConnector);
  // _document.setChanged();

  Refresh();
}

void MyFrame::OnNewRect(wxCommandEvent &event) {
  // KRectWithAnfasser *rect =
  //     (KRectWithAnfasser *)_document.createFigure(FIGURETYPE_RECT, _canvas);
  // rect->setX(10);
  // rect->setY(10);
  // rect->setWidth(100);
  // rect->setHeight(200);
  // rect->initializeAnfasserRects();
  // rect->initializeCollectorAttachmentRects();
  // _document.addFigure(rect);
  // _document.setChanged();

  Refresh();
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

void MyFrame::OnDeleteFigure(wxCommandEvent &event) {
  //_document.deleteSelectedFigure();

  Refresh();
}
