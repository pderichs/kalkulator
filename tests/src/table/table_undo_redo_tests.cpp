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

class TableUndoRedoTests : public testing::Test {
protected:
  void SetUp() override {
    ValueConverter::set_execution_context(&_execution_context);
    _document = std::make_shared<TableWorkbookDocument>(&_sink);
    prepare_execution_context(&_execution_context, _document);
  }

protected:
  TableWorkbookDocumentPtr _document;
  LispExecutionContext _execution_context;
  TestEventSink _sink;
};

TEST_F(TableUndoRedoTests, SingleStepUndoTest) {
  _document->select_cell(Location(0, 1));
  _document->update_content_current_cells("42", generate_update_id());

  _document->undo(generate_update_id());

  auto cell = _document->get_cell(Location(0, 1));
  EXPECT_EQ(cell->get_formula_content(), "");
}

TEST_F(TableUndoRedoTests, SingleStepUndoRedoTest) {
  _document->select_cell(Location(0, 1));
  _document->update_content_current_cells("42", generate_update_id());

  _document->undo(generate_update_id());

  auto cell = _document->get_cell(Location(0, 1));
  EXPECT_EQ(cell->get_formula_content(), "");

  _document->redo(generate_update_id());
  EXPECT_EQ(cell->get_formula_content(), "42");
}

TEST_F(TableUndoRedoTests, TwoUndoSteps) {
  _document->select_cell(Location(0, 1));
  _document->update_content_current_cells("42", generate_update_id());
  _document->update_content_current_cells("Test123", generate_update_id());

  _document->undo(generate_update_id());
  auto cell = _document->get_cell(Location(0, 1));
  EXPECT_EQ(cell->get_formula_content(), "42");

  _document->undo(generate_update_id());
  EXPECT_EQ(cell->get_formula_content(), "");
}
