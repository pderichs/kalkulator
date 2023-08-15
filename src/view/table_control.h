#ifndef TABLECONTROL_INCLUDED
#define TABLECONTROL_INCLUDED

#include <wx/sizer.h>
#include <wx/wx.h>

#include "../model/event_sink.h"
#include "../model/table/table_workbook_document.h"
#include "cells_view_control.h"
#include "location.h"

class TableControl : public wxWindow {
public:
  TableControl(TableWorkbookDocument *document, EventSink *event_sink,
               wxWindow *parent, wxWindowID id = wxID_ANY,
               const wxPoint &pos = wxDefaultPosition,
               const wxSize &size = wxDefaultSize,
               long style = wxTAB_TRAVERSAL);

  virtual ~TableControl();
  void OnCellUpdate(const Location &location);

  void OnSetFocus(wxFocusEvent& event);
protected:
  void Init();

  void CreateHeaderControls();
  void CreateCellsViewControl();

private:
  EventSink *_event_sink;
  TableWorkbookDocument *_document;
  wxSizer *_top_sizer;
  wxSizer *_row_cell_view_sizer;
  CellsViewControl *_cells_view_control;

  // TODO Placeholders
  wxButton *_column_header_control;
  wxButton *_row_header_control;
};

#endif
