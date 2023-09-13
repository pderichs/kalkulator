#include "table_cell_format_dlg.h"
#include <wx/colordlg.h>
#include <wx/fontenum.h>
#include <wx/wx.h>

TableCellFormatDlg::TableCellFormatDlg(wxWindow *parent,
                                       const wxFont &cell_view_font,
                                       wxWindowID id, const wxString &title,
                                       const wxPoint &pos, const wxSize &size,
                                       long style)
    : wxDialog(parent, id, title, pos, size, style) {
  _cell_view_font = cell_view_font;

  this->SetSizeHints(wxDefaultSize, wxDefaultSize);

  wxBoxSizer *bSizerGlobal;
  bSizerGlobal = new wxBoxSizer(wxVERTICAL);

  wxBoxSizer *bSizerStyle;
  bSizerStyle = new wxBoxSizer(wxHORIZONTAL);

  m_chkBold = new wxCheckBox(this, wxID_ANY, wxT("Bold"), wxDefaultPosition,
                             wxDefaultSize, 0);
  bSizerStyle->Add(m_chkBold, 0, wxALL, 5);

  m_chkItalic = new wxCheckBox(this, wxID_ANY, wxT("Italic"), wxDefaultPosition,
                               wxDefaultSize, 0);
  bSizerStyle->Add(m_chkItalic, 0, wxALL, 5);

  m_chkUnderlined = new wxCheckBox(this, wxID_ANY, wxT("Underlined"),
                                   wxDefaultPosition, wxDefaultSize, 0);
  bSizerStyle->Add(m_chkUnderlined, 0, wxALL, 5);

  bSizerGlobal->Add(bSizerStyle, 0, wxEXPAND, 5);

  wxBoxSizer *bSizerFontName;
  bSizerFontName = new wxBoxSizer(wxHORIZONTAL);

  m_lblFontName = new wxStaticText(this, wxID_ANY, wxT("Font Name:"),
                                   wxDefaultPosition, wxDefaultSize, 0);
  m_lblFontName->Wrap(-1);
  bSizerFontName->Add(m_lblFontName, 0, wxALL, 5);

  m_cmbFontName = new wxComboBox(this, wxID_ANY, _cell_view_font.GetFaceName(),
                                 wxDefaultPosition, wxDefaultSize, 0, NULL, 0);
  bSizerFontName->Add(m_cmbFontName, 1, wxALL, 5);

  bSizerGlobal->Add(bSizerFontName, 0, wxEXPAND, 5);

  wxBoxSizer *bSizerFontSize;
  bSizerFontSize = new wxBoxSizer(wxHORIZONTAL);

  m_lblFontSize = new wxStaticText(this, wxID_ANY, wxT("Font Size:"),
                                   wxDefaultPosition, wxDefaultSize, 0);
  m_lblFontSize->Wrap(-1);
  bSizerFontSize->Add(m_lblFontSize, 0, wxALL, 5);

  m_spnFontSize =
      new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
                     wxDefaultSize, wxSP_ARROW_KEYS, 0, 50, 0);
  m_spnFontSize->SetValue(_cell_view_font.GetPointSize());
  bSizerFontSize->Add(m_spnFontSize, 0, wxALL, 5);

  bSizerGlobal->Add(bSizerFontSize, 0, wxEXPAND, 5);

  wxBoxSizer *bSizerBackgroundColor;
  bSizerBackgroundColor = new wxBoxSizer(wxHORIZONTAL);

  m_lblBackgroundColor =
      new wxStaticText(this, wxID_ANY, wxT("Background Color:"),
                       wxDefaultPosition, wxDefaultSize, 0);
  m_lblBackgroundColor->Wrap(-1);
  bSizerBackgroundColor->Add(m_lblBackgroundColor, 0, wxALL, 5);

  m_lblBackgroundColorVisualization = new wxStaticText(
      this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
  m_lblBackgroundColorVisualization->Wrap(-1);
  bSizerBackgroundColor->Add(m_lblBackgroundColorVisualization, 1, wxALL, 5);

  m_btnSelectBackgroundColor = new wxButton(
      this, wxID_ANY, wxT("..."), wxDefaultPosition, wxDefaultSize, 0);
  bSizerBackgroundColor->Add(m_btnSelectBackgroundColor, 0, wxALL, 5);

  bSizerGlobal->Add(bSizerBackgroundColor, 0, wxEXPAND, 5);

  wxBoxSizer *bSizerForegroundColor;
  bSizerForegroundColor = new wxBoxSizer(wxHORIZONTAL);

  m_lblForegroundColor =
      new wxStaticText(this, wxID_ANY, wxT("Foreground Color:"),
                       wxDefaultPosition, wxDefaultSize, 0);
  m_lblForegroundColor->Wrap(-1);
  bSizerForegroundColor->Add(m_lblForegroundColor, 0, wxALL, 5);

  m_lblForegroundColorVisualization = new wxStaticText(
      this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
  m_lblForegroundColorVisualization->Wrap(-1);
  bSizerForegroundColor->Add(m_lblForegroundColorVisualization, 1, wxALL, 5);

  m_btnSelectForegroundColor = new wxButton(
      this, wxID_ANY, wxT("..."), wxDefaultPosition, wxDefaultSize, 0);
  bSizerForegroundColor->Add(m_btnSelectForegroundColor, 0, wxALL, 5);

  bSizerGlobal->Add(bSizerForegroundColor, 1, wxEXPAND, 5);

  wxBoxSizer *bSizerDlgButtons;
  bSizerDlgButtons = new wxBoxSizer(wxHORIZONTAL);

  bSizerDlgButtons->Add(0, 0, 1, wxEXPAND, 5);

  m_btnOK = new wxButton(this, wxID_ANY, wxT("OK"), wxDefaultPosition,
                         wxDefaultSize, 0);
  bSizerDlgButtons->Add(m_btnOK, 0, wxALL, 5);

  m_btnCancel = new wxButton(this, wxID_ANY, wxT("Cancel"), wxDefaultPosition,
                             wxDefaultSize, 0);
  bSizerDlgButtons->Add(m_btnCancel, 0, wxALL, 5);

  bSizerGlobal->Add(bSizerDlgButtons, 0, wxEXPAND, 5);

  this->SetSizer(bSizerGlobal);
  this->Layout();

  this->Centre(wxBOTH);

  m_btnOK->Bind(wxEVT_BUTTON, &TableCellFormatDlg::OnOK, this);
  m_btnCancel->Bind(wxEVT_BUTTON, &TableCellFormatDlg::OnCancel, this);
  m_btnSelectBackgroundColor->Bind(
      wxEVT_BUTTON, &TableCellFormatDlg::OnSelectBackgroundColor, this);
  m_btnSelectForegroundColor->Bind(
      wxEVT_BUTTON, &TableCellFormatDlg::OnSelectForegroundColor, this);

  InitializeFontCombo();
}

TableCellFormatDlg::~TableCellFormatDlg() {}

void TableCellFormatDlg::InitializeFontCombo() {
  m_cmbFontName->Insert(wxFontEnumerator::GetFacenames(), 0);
}

void TableCellFormatDlg::OnOK(wxCommandEvent &WXUNUSED(event)) {
  EndModal(wxID_OK);
}

void TableCellFormatDlg::OnCancel(wxCommandEvent &WXUNUSED(event)) {
  EndModal(wxID_CANCEL);
}

TableCellFormat TableCellFormatDlg::GetFormat() const {
  TableCellFormat result;

  if (_background_color) {
    result.background_color = wxColourToTableCellColor(*_background_color);
  }

  if (_foreground_color) {
    result.foreground_color = wxColourToTableCellColor(*_foreground_color);
  }

  if (!m_cmbFontName->GetValue().IsEmpty()) {
    result.font_name = (const char *)m_cmbFontName->GetValue();
  }

  if (m_chkBold->GetValue()) {
    result.bold = true;
  }

  if (m_chkItalic->GetValue()) {
    result.italic = true;
  }

  if (m_chkUnderlined->GetValue()) {
    result.underlined = true;
  }

  result.font_size = static_cast<size_t>(m_spnFontSize->GetValue());

  return result;
}

TableCellColor
TableCellFormatDlg::wxColourToTableCellColor(const wxColour &color) const {
  TableCellColor result;

  result.r = color.Red();
  result.b = color.Blue();
  result.g = color.Green();

  return result;
}

void TableCellFormatDlg::OnSelectForegroundColor(
    wxCommandEvent &WXUNUSED(event)) {
  wxColourDialog dlg(this);

  if (dlg.ShowModal() == wxID_OK) {
    _foreground_color = dlg.GetColourData().GetColour();

    m_lblForegroundColorVisualization->SetBackgroundColour(*_foreground_color);
  }
}

void TableCellFormatDlg::OnSelectBackgroundColor(
    wxCommandEvent &WXUNUSED(event)) {
  wxColourDialog dlg(this);

  if (dlg.ShowModal() == wxID_OK) {
    _background_color = dlg.GetColourData().GetColour();

    m_lblBackgroundColorVisualization->SetBackgroundColour(*_background_color);
  }
}
