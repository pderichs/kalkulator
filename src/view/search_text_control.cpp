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

#include "search_text_control.h"

SearchTextControl::SearchTextControl(wxWindow *parent,
                                     EventSink *event_sink,
                                     wxWindowID id,
                                     const wxString &value,
                                     const wxPoint &pos,
                                     const wxSize &size,
                                     long style,
                                     const wxValidator &validator,
                                     const wxString &name) : InputTextControl(
    parent,
    id,
    value,
    pos,
    size,
    style,
    validator,
    name), _event_sink(event_sink) {}

void SearchTextControl::OnEnter(wxKeyEvent &WXUNUSED(event)) {
  _event_sink->send_event(SEARCH_TEXT_CONTROL_SEARCH_TERM_COMMAND, {});
}
