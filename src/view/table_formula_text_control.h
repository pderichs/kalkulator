#ifndef TABLE_FORMULA_TEXT_CONTROL_INCLUDED
#define TABLE_FORMULA_TEXT_CONTROL_INCLUDED

#include <wx/wx.h>

#include "../model/event_sink.h"

class TableFormulaTextControl : public wxTextCtrl {
public:
  TableFormulaTextControl(EventSink *event_sink, wxWindow *parent,
                          wxWindowID id, const wxString &value = wxEmptyString,
                          const wxPoint &pos = wxDefaultPosition,
                          const wxSize &size = wxDefaultSize, long style = 0,
                          const wxValidator &validator = wxDefaultValidator,
                          const wxString &name = wxTextCtrlNameStr);
  void OnKeyPress(wxKeyEvent &event);

private:
  EventSink *_event_sink;
};

#endif
