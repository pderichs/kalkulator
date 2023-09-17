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

#include "table_formula_text_control.h"
#include "../model/event_sink.h"

TableFormulaTextControl::TableFormulaTextControl(
    EventSink *event_sink, wxWindow *parent, wxWindowID id,
    const wxString &value, const wxPoint &pos, const wxSize &size, long style,
    const wxValidator &validator, const wxString &name)
    : wxTextCtrl(parent, id, value, pos, size, style, validator, name),
      _event_sink(event_sink) {
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
    _event_sink->send_event(FORMULA_CANCEL, {});
    break;
  }

  event.Skip();
}
