#ifndef TABLECONTROL_INCLUDED
#define TABLECONTROL_INCLUDED

#include <wx/panel.h>

class TableControl : public wxPanel {
public:
  TableControl(wxWindow *parent, wxWindowID id = wxID_ANY,
               const wxPoint &pos = wxDefaultPosition,
               const wxSize &size = wxDefaultSize, long style = wxTAB_TRAVERSAL,
               const wxString &name = wxPanelNameStr);

  void OnPaint(wxPaintEvent& event);
};

#endif
