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
#include <any>
#include <memory>

TEST(TableCellRangeTests, CellRangeTest1) {
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

  const auto &cell = document->get_cell(Location(1, 0));
  EXPECT_TRUE(cell);
  LispValuePtr value = cell->lisp_value();
  EXPECT_TRUE(value->is_function());
  EXPECT_EQ(cell->visible_content(), actual_sum_str.str());
}

TEST(TableCellRangeTests, RangeEqualityTest1) {
  // Test setup
  TestEventSink sink;
  LispExecutionContext execution_context;
  ValueConverter::set_execution_context(&execution_context);
  TableWorkbookDocumentPtr document =
      std::make_shared<TableWorkbookDocument>(&sink);
  prepare_execution_context(&execution_context, document);

  // Prepare cell content
  for (int r = 0; r < 5; r++) {
    for (int c = 0; c < 10; c++) {
      document->select_cell(Location(c, r));
      document->update_content_current_cell("2");
    }
  }

  for (int r = 0; r < 2; r++) {
    for (int c = 20; c < 22; c++) {
      document->select_cell(Location(c, r));
      document->update_content_current_cell("2");
    }
  }

  // Formula
  document->select_cell(Location(0, 50));
  document->update_content_current_cell(
      "=(= (cell_range 0 0 4 10) (cell_range 0 20 1 21))");

  // Cell content must match source cell 1
  const auto &cell = document->get_cell(Location(0, 50));
  EXPECT_EQ(cell->visible_content(), "TRUE");

  // Change one cell
  document->select_cell(Location(0, 0));
  document->update_content_current_cell("3");
  EXPECT_EQ(cell->visible_content(), "FALSE");

  document->select_cell(Location(0, 0));
  document->update_content_current_cell("2");
  EXPECT_EQ(cell->visible_content(), "TRUE");

  // Change cell in other range
  document->select_cell(Location(20, 0));
  document->update_content_current_cell("Hello");
  EXPECT_EQ(cell->visible_content(), "FALSE");
}