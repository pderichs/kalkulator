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

class TableSearchTests : public TableTest {
protected:
  void SetUp() override {
    TableTest::SetUp();

    _document->add_sheet("Testsheet");
    _document->select_sheet_by_name("Testsheet");
    _document->select_cell(Location(0, 1));
    _document->update_content_current_cells("42", generate_update_id());

    _document->select_sheet_by_name("Sheet 1");
    _document->select_cell(Location(0, 2));
    _document->update_content_current_cells("42", generate_update_id());
  }
};

TEST_F(TableSearchTests, TableSearchTest1) {
  auto result = _document->search_sheets("42");

  EXPECT_EQ(result.size(), 2);
  TableSearchResultItem item = result[0];
  EXPECT_EQ(item.sheet->name(), "Sheet 1");
  EXPECT_EQ(item.location, Location(0, 2));

  item = result[1];
  EXPECT_EQ(item.sheet->name(), "Testsheet");
  EXPECT_EQ(item.location, Location(0, 1));
}

TEST_F(TableSearchTests, TableReplaceTest1) {
  auto result = _document->search_sheets("42");

  EXPECT_EQ(result.size(), 2);
  auto item = result[1];

  _document->replace_search_item(item, "42", "hello123", generate_update_id());

  auto cell = item.sheet->get_cell(item.location);
  EXPECT_EQ(cell->visible_content(), "hello123");
}
