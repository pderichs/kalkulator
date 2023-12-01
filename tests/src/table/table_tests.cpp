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

#include "gtest/gtest.h"
#include "mock_function_context.h"
#include "table_test.h"

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

class TableTests: public TableTest {
protected:
  void SetUp() override {
    TableTest::SetUp();

    _fct = std::make_shared<TestFunctionContext>();
    _execution_context.add_function("test-function", _fct, true);
  }

protected:
  std::shared_ptr<TestFunctionContext> _fct;
};

TEST_F(TableTests,
     FunctionsWithCellReferencesAreCalledAsExpectedWithinTheSameUpdateId) {
  UpdateIdType update_id = generate_update_id();

  // Prepare test sheet data
  _document->select_cell(Location(0, 0));
  _document->update_content_current_cells("foo", update_id);

  // First cell referencing the value
  _document->select_cell(Location(0, 1));
  _document->update_content_current_cells("=(test-function (cell 0 0))",
                                         update_id);

  // Another cell references value
  _document->select_cell(Location(0, 2));
  _document->update_content_current_cells("=(cell 1 0)", update_id);

  EXPECT_EQ(_fct->calls(), 1);

  _fct->reset_calls();

  auto cell = _document->get_cell(Location(0, 1));
  EXPECT_EQ(cell->visible_content(), "foo");

  cell = _document->get_cell(Location(0, 2));
  EXPECT_EQ(cell->visible_content(), "foo");

  update_id = generate_update_id();

  // Update of value cell should trigger only once with new update_id
  _document->select_cell(Location(0, 0));
  _document->update_content_current_cells("bar", update_id);

  EXPECT_EQ(_fct->calls(), 1);
}

TEST_F(TableTests, UpdateWorksOnAllSelectedCells) {
  std::shared_ptr<TestFunctionContext> fct =
      std::make_shared<TestFunctionContext>();
  _execution_context.add_function("test-function", fct, true);

  // Prepare test sheet data
  _document->select_cell(Location(0, 0));
  _document->selection_toggle_additional_cell(Location(10, 10));
  _document->selection_toggle_additional_cell(Location(3, 2));

  _document->update_content_current_cells("foobar", generate_update_id());

  EXPECT_EQ(_document->get_cell(Location(0, 0))->visible_content(), "foobar");
  EXPECT_EQ(_document->get_cell(Location(10, 10))->visible_content(), "foobar");
  EXPECT_EQ(_document->get_cell(Location(3, 2))->visible_content(), "foobar");
}
