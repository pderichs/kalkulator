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

  ~TableCellFormatDlg();

  void InitializeFontCombo();

  TableCellFormat GetFormat() const;

  TableCellColor wxColourToTableCellColor(const wxColour &color) const;
  void OnSelectForegroundColor(wxCommandEvent &WXUNUSED(event));
  void OnSelectBackgroundColor(wxCommandEvent &WXUNUSED(event));

private:
  std::optional<wxColour> _background_color;
  std::optional<wxColour> _foreground_color;

  wxFont _cell_view_font;
  std::optional<TableCellFormat> _cell_format;
};

#endif
