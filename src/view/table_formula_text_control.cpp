#include "table_formula_text_control.h"

TableFormulaTextControl::TableFormulaTextControl(
    EventSink *event_sink, wxWindow *parent, wxWindowID id,
    const wxString &value, const wxPoint &pos, const wxSize &size, long style,
    const wxValidator &validator, const wxString &name)
    : wxTextCtrl(parent, id, value, pos, size, style, validator, name) {
  _event_sink = event_sink;

  Bind(wxEVT_CHAR_HOOK, &TableFormulaTextControl::OnKeyPress, this);
}

void TableFormulaTextControl::OnKeyPress(wxKeyEvent &event) {
  // Handle the keypress event here
  int keyCode = event.GetKeyCode();

  // Example: Print the keycode to the console
  wxPrintf("Formula control: Key pressed: %d\n", keyCode);

  event.Skip();
}
