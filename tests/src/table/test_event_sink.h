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

#ifndef KALKULATOR_TESTS_SRC_TABLE_TEST_EVENT_SINK_H
#define KALKULATOR_TESTS_SRC_TABLE_TEST_EVENT_SINK_H

#include "event_sink.h"
#include <tuple>

class TestEventSink : public EventSink {
public:
  TestEventSink() = default;
  ~TestEventSink() override = default;

  void send_event(TableEvent event_id, std::any param) override {
    std::ignore = event_id;
    std::ignore = param;
  }
};

#endif//KALKULATOR_TESTS_SRC_TABLE_TEST_EVENT_SINK_H
