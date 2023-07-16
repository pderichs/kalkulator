#ifndef TABLE_FORMULA_TEXT_CONTROL_INCLUDED
#define TABLE_FORMULA_TEXT_CONTROL_INCLUDED

#include <wx/textctrl.h>

class TableFormulaTextControl : public wxTextCtrl {
public:
  TableFormulaTextControl(wxWindow *parent, wxWindowID id,
                          const wxString &value = wxEmptyString,
                          const wxPoint &pos = wxDefaultPosition,
                          const wxSize &size = wxDefaultSize, long style = 0,
                          const wxValidator &validator = wxDefaultValidator,
                          const wxString &name = wxTextCtrlNameStr);
};

#endif
