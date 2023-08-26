#include "table_formula_text_control.h"
#include "../model/event_sink.h"

TableFormulaTextControl::TableFormulaTextControl(
    EventSink *event_sink, wxWindow *parent, wxWindowID id,
    const wxString &value, const wxPoint &pos, const wxSize &size, long style,
    const wxValidator &validator, const wxString &name)
    : wxTextCtrl(parent, id, value, pos, size, style, validator, name) {
  _event_sink = event_sink;

  Bind(wxEVT_CHAR_HOOK, &TableFormulaTextControl::OnKeyPress, this);
}

void TableFormulaTextControl::OnKeyPress(wxKeyEvent &event) {
  int keyCode = event.GetKeyCode();

  switch (keyCode) {
  case WXK_RETURN:
    if (IsModified()) {
      std::string content = (const char *)GetValue();
      _event_sink->send_event(FORMULA_UPDATE, content);
    }
    break;

  case WXK_ESCAPE:
    // Does nothing. Moved handling to main frame.
    // if (IsModified()) {
    //   Undo();
    // }

    _event_sink->send_event(FORMULA_CANCEL, {});
    break;
  }

  event.Skip();
}
