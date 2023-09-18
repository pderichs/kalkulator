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

#include "table_cell_format_dlg.h"
#include <wx/colordlg.h>
#include <wx/fontenum.h>
#include <wx/wx.h>

TableCellFormatDlg::TableCellFormatDlg(
    wxWindow *parent, const std::optional<TableCellFormat> &cell_format,
    const wxFont &cell_view_font, wxWindowID id, const wxString &title,
    const wxPoint &pos, const wxSize &size, long style)
    : wxDialog(parent, id, title, pos, size, style), m_chkBold(nullptr),
      m_chkItalic(nullptr), m_chkUnderlined(nullptr), m_lblFontName(nullptr),
      m_cmbFontName(nullptr), m_lblFontSize(nullptr), m_spnFontSize(nullptr),
      m_lblBackgroundColor(nullptr), m_lblBackgroundColorVisualization(nullptr),
      m_btnSelectBackgroundColor(nullptr), m_lblForegroundColor(nullptr),
      m_lblForegroundColorVisualization(nullptr),
      m_btnSelectForegroundColor(nullptr), m_btnOK(nullptr),
      m_btnCancel(nullptr), _background_color(), _foreground_color(),
      _cell_view_font(), _cell_format() {
  _cell_format = cell_format;
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

  m_cmbFontName =
      new wxComboBox(this, wxID_ANY, _cell_view_font.GetFaceName(),
                     wxDefaultPosition, wxDefaultSize, 0, nullptr, 0);
  bSizerFontName->Add(m_cmbFontName, 1, wxALL, 5);

  bSizerGlobal->Add(bSizerFontName, 0, wxEXPAND, 5);

  wxBoxSizer *bSizerFontSize;
  bSizerFontSize = new wxBoxSizer(wxHORIZONTAL);

  m_lblFontSize = new wxStaticText(this, wxID_ANY, wxT("Font Size:"),
                                   wxDefaultPosition, wxDefaultSize, 0);
  m_lblFontSize->Wrap(-1);
  bSizerFontSize->Add(m_lblFontSize, 0, wxALL, 5);

  size_t font_size;
  if (_cell_format && _cell_format->font_size) {
    font_size = *(_cell_format->font_size);
  } else {
    font_size = _cell_view_font.GetPointSize();
  }
  m_spnFontSize =
      new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
                     wxDefaultSize, wxSP_ARROW_KEYS, 0, 50, 0);
  m_spnFontSize->SetValue(font_size);
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

  UpdateByTableCellFormat();
}

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

wxColour TableCellFormatDlg::tableCellColorTowxColour(
    const TableCellColor &color) const {
  return wxColour(color.r, color.g, color.b);
}

void TableCellFormatDlg::OnSelectForegroundColor(
    wxCommandEvent &WXUNUSED(event)) {
  wxColourDialog dlg(this);

  if (dlg.ShowModal() == wxID_OK) {
    SetForegroundColor(dlg.GetColourData().GetColour());
  }
}

void TableCellFormatDlg::OnSelectBackgroundColor(
    wxCommandEvent &WXUNUSED(event)) {
  wxColourDialog dlg(this);

  if (dlg.ShowModal() == wxID_OK) {
    SetBackgroundColor(dlg.GetColourData().GetColour());
  }
}

void TableCellFormatDlg::UpdateByTableCellFormat() {
  if (!_cell_format) {
    return;
  }

  if (_cell_format->background_color) {
    wxColour color = tableCellColorTowxColour(*_cell_format->background_color);
    SetBackgroundColor(color);
  }

  if (_cell_format->foreground_color) {
    wxColour color = tableCellColorTowxColour(*_cell_format->foreground_color);
    SetForegroundColor(color);
  }

  if (_cell_format->font_name) {
    m_cmbFontName->SetValue(*_cell_format->font_name);
  }

  if (_cell_format->bold) {
    m_chkBold->SetValue(*_cell_format->bold);
  }

  if (_cell_format->italic) {
    m_chkItalic->SetValue(*_cell_format->italic);
  }

  if (_cell_format->underlined) {
    m_chkUnderlined->SetValue(*_cell_format->underlined);
  }
}

void TableCellFormatDlg::SetBackgroundColor(const wxColour &color) {
  _background_color = color;
  m_lblBackgroundColorVisualization->SetBackgroundColour(*_background_color);
}

void TableCellFormatDlg::SetForegroundColor(const wxColour &color) {
  _foreground_color = color;
  m_lblForegroundColorVisualization->SetBackgroundColour(*_foreground_color);
}
