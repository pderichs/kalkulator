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

#ifndef INPUT_TEXT_CONTROL_H
#define INPUT_TEXT_CONTROL_H

#include <wx/wx.h>
#include <wx/textctrl.h>

class InputTextControl : public wxTextCtrl {
public:
  InputTextControl(wxWindow *parent,
                   wxWindowID id,
                   const wxString &value,
                   const wxPoint &pos,
                   const wxSize &size,
                   long style,
                   const wxValidator &validator,
                   const wxString &name);

  void OnKeyPress(wxKeyEvent &event);

  virtual void OnEnter(wxKeyEvent &event) = 0;
  virtual void OnEscape(wxKeyEvent &event) = 0;
};

#endif //INPUT_TEXT_CONTROL_H
