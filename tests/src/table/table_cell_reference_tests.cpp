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
#include "table_test.h"
#include <any>
#include <memory>

class TableCellReferenceTests: public TableTest {};

TEST_F(TableCellReferenceTests, CellTest1) {
  // Put value in cell 0 0
  _document->select_cell(Location(0, 0));
  _document->update_content_current_cells("42", generate_update_id());

  // Put cell formula in cell 0 1
  _document->select_cell(Location(0, 1));
  _document->update_content_current_cells("=(cell 0 0)", generate_update_id());

  const auto &cell = _document->get_cell(Location(0, 1));
  EXPECT_TRUE(cell);
  LispValuePtr value = cell->lisp_value();
  EXPECT_TRUE(value->is_function());
  EXPECT_EQ(cell->visible_content(), "42");
}

TEST_F(TableCellReferenceTests, NestedReferenceTest1) {
  // Prepare formulas and cell content
  _document->select_cell(Location(0, 0));
  _document->update_content_current_cells("0", generate_update_id()); // Zeile

  _document->select_cell(Location(0, 1));
  _document->update_content_current_cells("5", generate_update_id()); // Spalte

  _document->select_cell(Location(5, 0)); // source cell 1
  _document->update_content_current_cells("Cell 1", generate_update_id());

  _document->select_cell(Location(6, 0)); // source cell 2
  _document->update_content_current_cells("Cell 2 - hello",
                                         generate_update_id());

  // Formula which contains cell reference defined by other cells.
  _document->select_cell(Location(0, 2));
  _document->update_content_current_cells(
      "=(cell (cell 0 0) (cell 1 0))", generate_update_id()); // Spalte

  // Cell content must match source cell 1
  const auto &cell = _document->get_cell(Location(0, 2));
  EXPECT_EQ(cell->visible_content(), "Cell 1");

  // Now update cell reference coordinate cell
  _document->select_cell(Location(0, 1));
  _document->update_content_current_cells("6", generate_update_id()); // Spalte

  // Cell content should now match source cell 2
  EXPECT_EQ(cell->visible_content(), "Cell 2 - hello");
}

TEST_F(TableCellReferenceTests, CallToCellWithStringParameterAsRow) {
  // Prepare formulas and cell content
  _document->select_cell(Location(0, 0));
  _document->update_content_current_cells("=(cell hello 1)",
                                         generate_update_id());

  // Cell content must match source cell 1
  const auto &cell = _document->get_cell(Location(0, 0));
  EXPECT_EQ(cell->visible_content(), "#PARAMERR");
}

TEST_F(TableCellReferenceTests, CallToCellWithStringParameterAsColumn) {
  // Prepare formulas and cell content
  _document->select_cell(Location(0, 0));
  _document->update_content_current_cells("=(cell 2 hello)",
                                         generate_update_id());

  // Cell content must match source cell 1
  const auto &cell = _document->get_cell(Location(0, 0));
  EXPECT_EQ(cell->visible_content(), "#PARAMERR");
}

TEST_F(TableCellReferenceTests, CellReferenceToOtherSheet) {
  // Prepare second sheet
  _document->add_sheet("Testsheet");
  _document->select_sheet_by_name("Testsheet");
  _document->select_cell(Location(0, 1));
  _document->update_content_current_cells("42", generate_update_id());

  // Prepare formulas and cell content
  _document->select_sheet_by_name("Sheet 1");
  _document->select_cell(Location(0, 0));
  _document->update_content_current_cells("=(cell \"Testsheet\" 1 0)",
                                         generate_update_id());

  // Cell content must match source cell
  const auto &cell = _document->get_cell(Location(0, 0));
  EXPECT_EQ(cell->visible_content(), "42");
}
