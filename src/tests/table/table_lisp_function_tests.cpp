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


#include <any>
#include <memory>
#include <sstream>
#include <tuple>

#include "../../model/table/lisp_execution_context_cell_range.h"
#include "../../model/table/lisp_execution_context_cell_reference.h"
#include "table_lisp_function_tests.h"
#include "tools.h"

int run_cell_tests1();

int run_cell_range_tests1();

class TestEventSink : public EventSink {
public:
  TestEventSink() = default;
  ~TestEventSink() override = default;

  void send_event(TableEvent event_id, std::any param) override {
    std::ignore = event_id;
    std::ignore = param;
  }
};

void prepare_execution_context(LispExecutionContext *execution_context,
                               TableWorkbookDocumentPtr document);

int run_table_lisp_function_tests() {
  RUN_TEST(run_cell_tests1);
  RUN_TEST(run_cell_range_tests1);

  return 0;
}

int run_cell_tests1() {
  // Test setup
  TestEventSink sink;
  LispExecutionContext execution_context;
  ValueConverter::set_execution_context(&execution_context);
  TableWorkbookDocumentPtr document =
      std::make_shared<TableWorkbookDocument>(&sink);
  prepare_execution_context(&execution_context, document);

  // Put value in cell 0 0
  document->select_cell(Location(0, 0));
  document->update_content_current_cell("42");

  // Put cell formula in cell 0 1
  document->select_cell(Location(0, 1));
  document->update_content_current_cell("=(cell 0 0)");

  const auto &opt_cell = document->get_cell(Location(0, 1));
  TEST_ASSERT(opt_cell);
  const auto &cell = *opt_cell;
  TEST_ASSERT(cell);
  LispValuePtr value = cell->lisp_value();
  TEST_ASSERT(value->is_function());
  TEST_ASSERT(cell->visible_content() == "42");

  return 0;
}

int run_cell_range_tests1() {
  // Test setup
  TestEventSink sink;
  LispExecutionContext execution_context;
  ValueConverter::set_execution_context(&execution_context);
  TableWorkbookDocumentPtr document =
      std::make_shared<TableWorkbookDocument>(&sink);
  prepare_execution_context(&execution_context, document);

  // Put random values in cells and store actual sum
  int actual_sum = 0;
  for (int n = 0; n <= 5; n++) {
    int rand = pdtools::generate_random_int_in_range(1, 20000);
    actual_sum += rand;
    document->select_cell(Location(0, n));
    std::stringstream ss;
    ss << rand;
    document->update_content_current_cell(ss.str());
  }

  std::stringstream actual_sum_str;
  actual_sum_str << actual_sum;

  // Put cell_range formula in cell 0 1 which sums up all values
  // in rows 0 - 5 of first column
  document->select_cell(Location(1, 0));
  document->update_content_current_cell("=(+ (cell_range 0 0 5 0))");

  const auto &opt_cell = document->get_cell(Location(1, 0));
  TEST_ASSERT(opt_cell);
  const auto &cell = *opt_cell;
  TEST_ASSERT(cell);
  LispValuePtr value = cell->lisp_value();
  TEST_ASSERT(value->is_function());
  TEST_ASSERT(cell->visible_content() == actual_sum_str.str());

  return 0;
}

void prepare_execution_context(LispExecutionContext *execution_context,
                               TableWorkbookDocumentPtr document) {
  execution_context->add_function(
      "cell", std::make_shared<LispExecutionContextCellReference>(document));
  execution_context->add_function(
      "cell_range", std::make_shared<LispExecutionContextCellRange>(document));
}
