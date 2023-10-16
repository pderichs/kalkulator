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

#include "table_test_tools.h"
#include "gtest/gtest.h"
#include "mock_function_context.h"

using ::testing::Return;

/**
 * Specialized test function execution context used for the cell reference /
 * cached result test below.
 *
 * NOTE Mocks won't fit here, since we need some part of the main function
 * context functionality here to e.g. register the required listeners.
 */
class TestFunctionContext : public LispFunctionExecutionContext {
public:
  TestFunctionContext() : _calls(0) {}
  ~TestFunctionContext() override = default;

  virtual LispValuePtr value(const LispValuePtrVector &func,
                             const LispExecutionContext &execution_context,
                             const std::any &context_param,
                             UpdateIdType update_id) override {
    auto params = extract_and_execute_params(func,
                                             execution_context,
                                             context_param,
                                             update_id);

    if (params.empty()) {
      return LispCommonValues::error_parameter_count();
    }

    _calls++;

    return params[0];
  }

  size_t calls() const { return _calls; }

  void reset_calls() { _calls = 0; }

private:
  size_t _calls;
  LispValuePtr _result;
};

TEST(TableTests,
     FunctionsWithCellReferencesAreCalledAsExpectedWithinTheSameUpdateId) {
  // Test setup
  LispExecutionContext execution_context;
  std::shared_ptr<TestFunctionContext> fct =
      std::make_shared<TestFunctionContext>();
  execution_context.add_function("test-function", fct, true);

  TestEventSink sink;
  ValueConverter::set_execution_context(&execution_context);
  TableWorkbookDocumentPtr document =
      std::make_shared<TableWorkbookDocument>(&sink);
  prepare_execution_context(&execution_context, document);

  UpdateIdType update_id = generate_update_id();

  // Prepare test sheet data
  document->select_cell(Location(0, 0));
  document->update_content_current_cells("foo", update_id);

  // First cell referencing the value
  document->select_cell(Location(0, 1));
  document->update_content_current_cells("=(test-function (cell 0 0))",
                                         update_id);

  // Another cell references value
  document->select_cell(Location(0, 2));
  document->update_content_current_cells("=(cell 1 0)", update_id);

  EXPECT_EQ(fct->calls(), 1);

  fct->reset_calls();

  auto cell = document->get_cell(Location(0, 1));
  EXPECT_EQ(cell->visible_content(), "foo");

  cell = document->get_cell(Location(0, 2));
  EXPECT_EQ(cell->visible_content(), "foo");

  update_id = generate_update_id();

  // Update of value cell should trigger only once with new update_id
  document->select_cell(Location(0, 0));
  document->update_content_current_cells("bar", update_id);

  EXPECT_EQ(fct->calls(), 1);
}

TEST(TableTests, UpdateWorksOnAllSelectedCells) {
  // Test setup
  LispExecutionContext execution_context;
  std::shared_ptr<TestFunctionContext> fct =
      std::make_shared<TestFunctionContext>();
  execution_context.add_function("test-function", fct, true);

  TestEventSink sink;
  ValueConverter::set_execution_context(&execution_context);
  TableWorkbookDocumentPtr document =
      std::make_shared<TableWorkbookDocument>(&sink);
  prepare_execution_context(&execution_context, document);

  // Prepare test sheet data
  document->select_cell(Location(0, 0));
  document->selection_toggle_additional_cell(Location(10, 10));
  document->selection_toggle_additional_cell(Location(3, 2));

  document->update_content_current_cells("foobar", generate_update_id());

  EXPECT_EQ(document->get_cell(Location(0, 0))->visible_content(), "foobar");
  EXPECT_EQ(document->get_cell(Location(10, 10))->visible_content(), "foobar");
  EXPECT_EQ(document->get_cell(Location(3, 2))->visible_content(), "foobar");
}
