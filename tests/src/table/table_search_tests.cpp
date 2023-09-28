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

TEST(TableSearchTests, TableSearchTest1) {
  // Test setup
  TestEventSink sink;
  LispExecutionContext execution_context;
  ValueConverter::set_execution_context(&execution_context);
  TableWorkbookDocumentPtr document =
      std::make_shared<TableWorkbookDocument>(&sink);
  prepare_execution_context(&execution_context, document);

  document->add_sheet("Testsheet");
  document->select_sheet_by_name("Testsheet");
  document->select_cell(Location(0, 1));
  document->update_content_current_cell("42");

  document->select_sheet_by_name("Sheet 1");
  document->select_cell(Location(0, 2));
  document->update_content_current_cell("42");

  auto result = document->search_sheets("42");

  EXPECT_EQ(result.size(), 2);
  TableSearchResultItem item = result[0];
  EXPECT_EQ(item.sheet->name, "Sheet 1");
  EXPECT_EQ(item.location, Location(0, 2));

  item = result[1];
  EXPECT_EQ(item.sheet->name, "Testsheet");
  EXPECT_EQ(item.location, Location(0, 1));
}