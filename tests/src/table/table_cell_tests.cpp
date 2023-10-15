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

#include "model/table/table_cell.h"
#include "gtest/gtest.h"

TEST(TableCellTests, ClearWorks) {
  TableCell cell(0, 0);

  cell.update_content("Hello", "Sheet 1", 1);

  EXPECT_EQ(cell.visible_content(), "Hello");
  EXPECT_TRUE(cell.has_content());

  cell.clear();

  EXPECT_TRUE(cell.visible_content().empty());
  EXPECT_FALSE(cell.has_content());
}