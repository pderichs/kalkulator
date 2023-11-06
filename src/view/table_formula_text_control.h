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

#ifndef TABLE_FORMULA_TEXT_CONTROL_INCLUDED
#define TABLE_FORMULA_TEXT_CONTROL_INCLUDED

#include "../model/event_sink.h"
#include "input_text_control.h"

/**
 * This provides a specialized text control for the formula.
 */
class TableFormulaTextControl : public InputTextControl {
public:
  TableFormulaTextControl(EventSink *event_sink, wxWindow *parent,
                          wxWindowID id, const wxString &value = wxEmptyString,
                          const wxPoint &pos = wxDefaultPosition,
                          const wxSize &size = wxDefaultSize, long style = 0,
                          const wxValidator &validator = wxDefaultValidator,
                          const wxString &name = wxTextCtrlNameStr);

  // Delete copy constructor and assignment operator
  TableFormulaTextControl(const TableFormulaTextControl &other) = delete;
  TableFormulaTextControl &
  operator=(const TableFormulaTextControl &other) = delete;

  void OnEnter(wxKeyEvent &event);
  void OnEscape(wxKeyEvent &event);

private:
  EventSink *_event_sink;
};

#endif
