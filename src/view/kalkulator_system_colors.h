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

#ifndef KALKULATOR_SYSTEM_COLORS_INCLUDED
#define KALKULATOR_SYSTEM_COLORS_INCLUDED

#include <memory>
#include <wx/wx.h>

typedef std::shared_ptr<wxPen> wxPenPtr;
typedef std::shared_ptr<wxBrush> wxBrushPtr;

/**
 * Contains the used colors for the (custom) UI controls.
 */
struct KalkulatorSystemColors {
  KalkulatorSystemColors()
      : window_color(wxSystemSettingsNative::GetColour(wxSYS_COLOUR_WINDOW)),
        window_text_color(wxSystemSettingsNative::GetColour(
            wxSYS_COLOUR_WINDOWTEXT)),
        button_face_color(wxSystemSettingsNative::GetColour(wxSYS_COLOUR_BTNFACE)),
        button_text_color(wxSystemSettingsNative::GetColour(wxSYS_COLOUR_BTNTEXT)),
        active_border_color(wxSystemSettingsNative::GetColour(
            wxSYS_COLOUR_HIGHLIGHT)),
        caption_grid_pen(std::make_shared<wxPen>(button_text_color)),
        grid_pen(std::make_shared<wxPen>(wxColour(100, 100, 100))),
        primary_cell_pen(std::make_shared<wxPen>(active_border_color)),
        window_brush(std::make_shared<wxBrush>(window_color)),
        caption_background_brush(std::make_shared<wxBrush>(button_face_color)),
        cell_selection_pen(primary_cell_pen), // TODO: selected cell pen and brush might be different in future
        cell_selection_brush(window_brush) {}

  wxColour window_color;
  wxColour window_text_color;
  wxColour button_face_color;
  wxColour button_text_color;
  wxColour active_border_color;

  wxPenPtr caption_grid_pen;
  wxPenPtr grid_pen;
  wxPenPtr primary_cell_pen;
  wxBrushPtr window_brush;
  wxBrushPtr caption_background_brush;
  wxPenPtr cell_selection_pen;
  wxBrushPtr cell_selection_brush;
};

typedef std::shared_ptr<KalkulatorSystemColors> KalkulatorSystemColorsPtr;

#endif
