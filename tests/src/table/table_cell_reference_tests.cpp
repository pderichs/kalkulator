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

TEST(TableCellTests, CellTest1) {
  // Test setup
  TestEventSink sink;
  LispExecutionContext execution_context;
  ValueConverter::set_execution_context(&execution_context);
  TableWorkbookDocumentPtr document =
      std::make_shared<TableWorkbookDocument>(&sink);
  prepare_execution_context(&execution_context, document);

  // Put value in cell 0 0
  document->select_cell(Location(0, 0));
  document->update_content_current_cells("42");

  // Put cell formula in cell 0 1
  document->select_cell(Location(0, 1));
  document->update_content_current_cells("=(cell 0 0)");

  const auto &cell = document->get_cell(Location(0, 1));
  EXPECT_TRUE(cell);
  LispValuePtr value = cell->lisp_value();
  EXPECT_TRUE(value->is_function());
  EXPECT_EQ(cell->visible_content(), "42");
}

TEST(TableCellTests, NestedReferenceTest1) {
  // Test setup
  TestEventSink sink;
  LispExecutionContext execution_context;
  ValueConverter::set_execution_context(&execution_context);
  TableWorkbookDocumentPtr document =
      std::make_shared<TableWorkbookDocument>(&sink);
  prepare_execution_context(&execution_context, document);

  // Prepare formulas and cell content
  document->select_cell(Location(0, 0));
  document->update_content_current_cells("0"); // Zeile

  document->select_cell(Location(0, 1));
  document->update_content_current_cells("5"); // Spalte

  document->select_cell(Location(5, 0)); // source cell 1
  document->update_content_current_cells("Cell 1");

  document->select_cell(Location(6, 0)); // source cell 2
  document->update_content_current_cells("Cell 2 - hello");

  // Formula which contains cell reference defined by other cells.
  document->select_cell(Location(0, 2));
  document->update_content_current_cells(
      "=(cell (cell 0 0) (cell 1 0))"); // Spalte

  // Cell content must match source cell 1
  const auto &cell = document->get_cell(Location(0, 2));
  EXPECT_EQ(cell->visible_content(), "Cell 1");

  // Now update cell reference coordinate cell
  document->select_cell(Location(0, 1));
  document->update_content_current_cells("6"); // Spalte

  // Cell content should now match source cell 2
  EXPECT_EQ(cell->visible_content(), "Cell 2 - hello");
}

TEST(TableCellTests, CallToCellWithStringParameterAsRow) {
  // Test setup
  TestEventSink sink;
  LispExecutionContext execution_context;
  ValueConverter::set_execution_context(&execution_context);
  TableWorkbookDocumentPtr document =
      std::make_shared<TableWorkbookDocument>(&sink);
  prepare_execution_context(&execution_context, document);

  // Prepare formulas and cell content
  document->select_cell(Location(0, 0));
  document->update_content_current_cells("=(cell hello 1)");

  // Cell content must match source cell 1
  const auto &cell = document->get_cell(Location(0, 0));
  EXPECT_EQ(cell->visible_content(), "#PARAMERR");
}

TEST(TableCellTests, CallToCellWithStringParameterAsColumn) {
  // Test setup
  TestEventSink sink;
  LispExecutionContext execution_context;
  ValueConverter::set_execution_context(&execution_context);
  TableWorkbookDocumentPtr document =
      std::make_shared<TableWorkbookDocument>(&sink);
  prepare_execution_context(&execution_context, document);

  // Prepare formulas and cell content
  document->select_cell(Location(0, 0));
  document->update_content_current_cells("=(cell 2 hello)");

  // Cell content must match source cell 1
  const auto &cell = document->get_cell(Location(0, 0));
  EXPECT_EQ(cell->visible_content(), "#PARAMERR");
}

TEST(TableCellTests, CellReferenceToOtherSheet) {
  // Test setup
  TestEventSink sink;
  LispExecutionContext execution_context;
  ValueConverter::set_execution_context(&execution_context);
  TableWorkbookDocumentPtr document =
      std::make_shared<TableWorkbookDocument>(&sink);
  prepare_execution_context(&execution_context, document);

  // Prepare second sheet
  document->add_sheet("Testsheet");
  document->select_sheet_by_name("Testsheet");
  document->select_cell(Location(0, 1));
  document->update_content_current_cells("42");

  // Prepare formulas and cell content
  document->select_sheet_by_name("Sheet 1");
  document->select_cell(Location(0, 0));
  document->update_content_current_cells("=(cell \"Testsheet\" 1 0)");

  // Cell content must match source cell
  const auto &cell = document->get_cell(Location(0, 0));
  EXPECT_EQ(cell->visible_content(), "42");
}
