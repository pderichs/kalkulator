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

#include "table_test.h"
#include "gtest/gtest.h"
#include <any>
#include <memory>

class TableCellRangeTests: public TableTest {};

TEST_F(TableCellRangeTests, CellRangeTest1) {
  // Put random values in cells and store actual sum
  int actual_sum = 0;
  for (int n = 0; n <= 5; n++) {
    int rand = pdtools::generate_random_int_in_range(1, 20000);
    actual_sum += rand;
    _document->select_cell(Location(0, n));
    std::stringstream ss;
    ss << rand;
    _document->update_content_current_cells(ss.str(), generate_update_id());
  }

  std::stringstream actual_sum_str;
  actual_sum_str << actual_sum;

  // Put cell_range formula in cell 0 1 which sums up all values
  // in rows 0 - 5 of first column
  _document->select_cell(Location(1, 0));
  _document->update_content_current_cells("=(+ (cell_range 0 0 5 0))", generate_update_id());

  const auto &cell = _document->get_cell(Location(1, 0));
  EXPECT_TRUE(cell);
  LispValuePtr value = cell->lisp_value();
  EXPECT_TRUE(value->is_function());
  EXPECT_EQ(cell->visible_content(), actual_sum_str.str());
}

TEST_F(TableCellRangeTests, RangeEqualityTest1) {
  // Prepare cell content
  for (int r = 0; r < 5; r++) {
    for (int c = 0; c < 10; c++) {
      _document->select_cell(Location(c, r));
      _document->update_content_current_cells("2", generate_update_id());
    }
  }

  for (int r = 0; r < 2; r++) {
    for (int c = 20; c < 22; c++) {
      _document->select_cell(Location(c, r));
      _document->update_content_current_cells("2", generate_update_id());
    }
  }

  // Formula
  _document->select_cell(Location(0, 50));
  _document->update_content_current_cells(
      "=(= (cell_range 0 0 4 10) (cell_range 0 20 1 21))", generate_update_id());

  // Cell content must match source cell 1
  const auto &cell = _document->get_cell(Location(0, 50));
  EXPECT_EQ(cell->visible_content(), "TRUE");

  // Change one cell
  _document->select_cell(Location(0, 0));
  _document->update_content_current_cells("3", generate_update_id());
  EXPECT_EQ(cell->visible_content(), "FALSE");

  _document->select_cell(Location(0, 0));
  _document->update_content_current_cells("2", generate_update_id());
  EXPECT_EQ(cell->visible_content(), "TRUE");

  // Change cell in other range
  _document->select_cell(Location(20, 0));
  _document->update_content_current_cells("Hello", generate_update_id());
  EXPECT_EQ(cell->visible_content(), "FALSE");
}
