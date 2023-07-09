#include "table_control.h"
#include <wx/dcclient.h>

TableControl::TableControl(wxWindow *parent, wxWindowID id, const wxPoint &pos,
                           const wxSize &size, long style,
                           const wxString &name): wxPanel(parent, id, pos, size, style, name) {
  Bind(wxEVT_PAINT, &TableControl::OnPaint, this);

  SetBackgroundColour(wxColour(*wxRED));
}

void TableControl::OnPaint(wxPaintEvent &event) { wxClientDC dc(this); }
