#ifndef KALKULATOR_SYSTEM_COLORS_INCLUDED
#define KALKULATOR_SYSTEM_COLORS_INCLUDED

#include <memory>
#include <wx/wx.h>

struct KalkulatorSystemColors {
  KalkulatorSystemColors() {
    window_color = wxSystemSettingsNative::GetColour(wxSYS_COLOUR_WINDOW);
    window_text_color =
        wxSystemSettingsNative::GetColour(wxSYS_COLOUR_WINDOWTEXT);
    button_face_color =
        wxSystemSettingsNative::GetColour(wxSYS_COLOUR_BTNFACE);
    button_text_color =
        wxSystemSettingsNative::GetColour(wxSYS_COLOUR_BTNTEXT);
    active_border_color =
        wxSystemSettingsNative::GetColour(wxSYS_COLOUR_HIGHLIGHT);

    window_brush = new wxBrush(window_color);
    caption_background_brush = new wxBrush(button_face_color);
    caption_grid_pen = new wxPen(button_text_color);
    grid_pen = new wxPen(wxColour(100, 100, 100));
    current_cell_pen = new wxPen(active_border_color);
  }

  ~KalkulatorSystemColors() {
    if (window_brush) {
      delete window_brush;
      window_brush = NULL;
    }

    if (grid_pen) {
      delete grid_pen;
      grid_pen = NULL;
    }

    if (caption_grid_pen) {
      delete caption_grid_pen;
      caption_grid_pen = NULL;
    }

    if (current_cell_pen) {
      delete current_cell_pen;
      current_cell_pen = NULL;
    }

    if (caption_background_brush) {
      delete caption_background_brush;
      caption_background_brush = NULL;
    }
  }

  wxPen *caption_grid_pen;
  wxPen *grid_pen;
  wxPen *current_cell_pen;
  wxBrush *window_brush;
  wxBrush *caption_background_brush;

  wxColour window_color;
  wxColour window_text_color;
  wxColour button_face_color;
  wxColour button_text_color;
  wxColour active_border_color;
};

typedef std::shared_ptr<KalkulatorSystemColors> KalkulatorSystemColorsPtr;

#endif
