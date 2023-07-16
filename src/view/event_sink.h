#ifndef EVENT_SINK_INCLUDED
#define EVENT_SINK_INCLUDED

enum TableEvent { FORMULA_UPDATE = 0 };

class EventSink {
public:
  virtual void send_event(TableEvent event_id, void *param) = 0;
};

#endif
