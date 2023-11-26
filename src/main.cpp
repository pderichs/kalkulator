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

#include <random>
#include "view/kalkulator_main_frame.h"

#if !defined(WX_PRECOMP)
#include <wx/wx.h>
#endif

#define WIDTH 1024
#define HEIGHT 768

class MyApp : public wxApp {
public:
  bool OnInit() override;
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
  srand(static_cast<unsigned>(time(nullptr)));

  wxString file_path;
  if (argc > 1) {
    file_path = argv[1];
  }

  auto *frame = new KalkulatorMainFrame(file_path);
  frame->Initialize();
  frame->SetSize(WIDTH, HEIGHT);
  frame->Show();

  return true;
}
