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

TableFormulaTextControl::TableFormulaTextControl(
    EventSink *event_sink, wxWindow *parent, wxWindowID id,
    const wxString &value, const wxPoint &pos, const wxSize &size, long style,
    const wxValidator &validator, const wxString &name)
    : InputTextControl(parent, id, value, pos, size, style, validator, name),
      _event_sink(event_sink) {
  Bind(wxEVT_CHAR_HOOK, &TableFormulaTextControl::OnKeyPress, this);
}

void TableFormulaTextControl::OnEnter(wxKeyEvent &WXUNUSED(event)) {
  std::string content = (const char *)GetValue();
  _event_sink->send_event(FORMULA_UPDATE, content);
}

void TableFormulaTextControl::OnEscape(wxKeyEvent &WXUNUSED(event)) {
  _event_sink->send_event(FORMULA_CANCEL, {});
}
