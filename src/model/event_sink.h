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
  HEADER_GOT_FOCUS,
  NAVIGATE_SEARCH_RESULT_ITEM,
  MESSAGE_BOX,
  EDIT_CELL,
};

class EventSink {
public:
  EventSink() = default;
  virtual ~EventSink() = default;

  virtual void send_event(TableEvent event_id, std::any param) = 0;
};

#endif
