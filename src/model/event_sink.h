#ifndef EVENT_SINK_INCLUDED
#define EVENT_SINK_INCLUDED

#include <any>

enum TableEvent {
  FORMULA_UPDATE = 0,
  CELL_UPDATED,
  CURRENT_CELL_LOCATION_UPDATED,
  CELL_VIEW_SCROLL_EVENT,
  SHEET_SELECTION_UPDATED,
  COLUMN_WIDTH_UPDATED,
  ROW_HEIGHT_UPDATED,
  FORMULA_CANCEL,
};

class EventSink {
public:
  EventSink() = default;
  virtual ~EventSink() = default;

  virtual void send_event(TableEvent event_id, std::any param) = 0;
};

#endif
