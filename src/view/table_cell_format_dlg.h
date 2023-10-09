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

#ifndef TABLE_CELL_FORMAT_DLG_INCLUDED
#define TABLE_CELL_FORMAT_DLG_INCLUDED

#include <optional>
#include <wx/artprov.h>
#include <wx/bitmap.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/colour.h>
#include <wx/combobox.h>
#include <wx/dialog.h>
#include <wx/font.h>
#include <wx/gdicmn.h>
#include <wx/icon.h>
#include <wx/image.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>
#include <wx/string.h>
#include <wx/xrc/xmlres.h>

#include "../model/table/table_cell_format.h"

/**
 * Provides the dialog to update a cell's format.
 */
class TableCellFormatDlg : public wxDialog {
protected:
  wxCheckBox *m_chkBold;
  wxCheckBox *m_chkItalic;
  wxCheckBox *m_chkUnderlined;
  wxStaticText *m_lblFontName;
  wxComboBox *m_cmbFontName;
  wxStaticText *m_lblFontSize;
  wxSpinCtrl *m_spnFontSize;
  wxStaticText *m_lblBackgroundColor;
  wxStaticText *m_lblBackgroundColorVisualization;
  wxButton *m_btnSelectBackgroundColor;
  wxStaticText *m_lblForegroundColor;
  wxStaticText *m_lblForegroundColorVisualization;
  wxButton *m_btnSelectForegroundColor;
  wxButton *m_btnOK;
  wxButton *m_btnCancel;

  void OnOK(wxCommandEvent &event);
  void OnCancel(wxCommandEvent &event);

public:
  TableCellFormatDlg(wxWindow *parent,
                     const std::optional<TableCellFormat> &cell_format,
                     const wxFont &cell_view_font, wxWindowID id = wxID_ANY,
                     const wxString &title = wxT("Format Cell"),
                     const wxPoint &pos = wxDefaultPosition,
                     const wxSize &size = wxSize(716, 451),
                     long style = wxDEFAULT_DIALOG_STYLE);

  ~TableCellFormatDlg() override = default;

  // Delete copy constructor and assignment operator
  TableCellFormatDlg(const TableCellFormatDlg &other) = delete;
  TableCellFormatDlg &operator=(const TableCellFormatDlg &other) = delete;

  void InitializeFontCombo();

  TableCellFormat GetFormat() const;

  TableCellColor wxColourToTableCellColor(const wxColour &color) const;
  wxColour tableCellColorTowxColour(const TableCellColor &color) const;

  void UpdateByTableCellFormat();

  void OnSelectForegroundColor(wxCommandEvent &WXUNUSED(event));
  void OnSelectBackgroundColor(wxCommandEvent &WXUNUSED(event));

  void SetBackgroundColor(const wxColour &color);
  void SetForegroundColor(const wxColour &color);

private:
  std::optional<wxColour> _background_color;
  std::optional<wxColour> _foreground_color;

  wxFont _cell_view_font;
  std::optional<TableCellFormat> _cell_format;
};

#endif
