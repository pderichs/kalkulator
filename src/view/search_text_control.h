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

#ifndef SEARCH_TEXT_CONTROL_H
#define SEARCH_TEXT_CONTROL_H

#include "input_text_control.h"
#include "model/event_sink.h"

class SearchTextControl: public InputTextControl {
public:
  SearchTextControl(wxWindow *parent,
                   EventSink *event_sink,
                   wxWindowID id, const wxString &value = wxEmptyString,
                   const wxPoint &pos = wxDefaultPosition,
                   const wxSize &size = wxDefaultSize, long style = 0,
                   const wxValidator &validator = wxDefaultValidator,
                   const wxString &name = wxTextCtrlNameStr);

  // Delete copy constructor and assignment operator
  SearchTextControl(const SearchTextControl &other) = delete;
  SearchTextControl &operator=(const SearchTextControl &other) = delete;

  void OnEnter(wxKeyEvent &event);

private:
  EventSink *_event_sink;
};

#endif //SEARCH_TEXT_CONTROL_H
