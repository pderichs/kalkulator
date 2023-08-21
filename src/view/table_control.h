#ifndef TABLECONTROL_INCLUDED
#define TABLECONTROL_INCLUDED

#include <wx/sizer.h>
#include <wx/wx.h>

#include "../model/event_sink.h"
#include "../model/table/table_workbook_document.h"
#include "cells_view_control.h"
#include "kalkulator_system_colors.h"
#include "location.h"
#include "table_column_headers_control.h"
#include "table_row_headers_control.h"

class TableControl : public wxWindow {
public:
  TableControl(KalkulatorSystemColorsPtr sys_colors,
               TableWorkbookDocumentPtr document, EventSink *event_sink,
               wxWindow *parent, wxWindowID id = wxID_ANY,
               const wxPoint &pos = wxDefaultPosition,
               const wxSize &size = wxDefaultSize,
               long style = wxTAB_TRAVERSAL);
  virtual ~TableControl();

  void OnCellUpdate(const Location &location);
  void OnSetFocus(wxFocusEvent &event);
  void update_scroll_positions(const Location &scroll_pos);

protected:
  void Init();
  void CreateHeaderControls();
  void CreateCellsViewControl();

private:
  EventSink *_event_sink;
  TableWorkbookDocumentPtr _document;

  wxSizer *_top_sizer;
  wxSizer *_row_cell_view_sizer;
  CellsViewControl *_cells_view_control;
  TableRowHeadersControl *_row_headers_control;
  TableColumnHeadersControl *_column_headers_control;
  KalkulatorSystemColorsPtr _sys_colors;
};

#endif
