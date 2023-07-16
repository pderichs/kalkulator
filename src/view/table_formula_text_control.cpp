#include "table_formula_text_control.h"

TableFormulaTextControl::TableFormulaTextControl(
    wxWindow *parent, wxWindowID id, const wxString &value,
    const wxPoint &pos, const wxSize &size,
    long style, const wxValidator &validator,
    const wxString &name)
    : wxTextCtrl(parent, id, value, pos, size, style, validator, name) {}
