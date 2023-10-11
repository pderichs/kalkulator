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

#include "model/table/table_cell_selection.h"
#include "gtest/gtest.h"
#include <algorithm>

TEST(TableCellSelectionTests, FirstPrimaryIsRow0Col0) {
  TableCellSelection selection;

  EXPECT_EQ(selection.primary(), Location(0, 0));
}

TEST(TableCellSelectionTests, SettingPrimaryWorks) {
  TableCellSelection selection;

  selection.set_primary(Location(42, 383));
  EXPECT_EQ(selection.primary(), Location(42, 383));

  EXPECT_EQ(selection.primary_x(), 42);
  EXPECT_EQ(selection.primary_y(), 383);
}

TEST(TableCellSelectionTests, AllLocationsWorks) {
  TableCellSelection selection;

  selection.set_primary(Location(42, 383));
  selection.add_cell(Location(334, 945390));
  selection.add_cell(Location(12, 1));
  selection.add_cell(Location(-2442, 1443));

  auto all = selection.all_locations();

  EXPECT_EQ(all.size(), 4);
  EXPECT_TRUE(all.find(Location(42, 383)) != all.end());
  EXPECT_TRUE(all.find(Location(334, 945390)) != all.end());
  EXPECT_TRUE(all.find(Location(12, 1)) != all.end());
  EXPECT_TRUE(all.find(Location(-2442, 1443)) != all.end());
}

TEST(TableCellSelectionTests, ReduceToPrimaryWorks) {
  TableCellSelection selection;

  selection.set_primary(Location(42, 383));
  selection.add_cell(Location(334, 945390));
  selection.add_cell(Location(12, 1));
  selection.add_cell(Location(-2442, 1443));

  selection.reduce_selection_to_primary();

  auto all = selection.all_locations();

  EXPECT_EQ(all.size(), 1);
  EXPECT_TRUE(all.find(Location(42, 383)) != all.end());
}

TEST(TableCellSelectionTests, PrimaryMoveUpWorks) {
  TableCellSelection selection;

  selection.set_primary(Location(42, 383));
  selection.add_cell(Location(334, 945390));
  selection.add_cell(Location(12, 1));
  selection.add_cell(Location(-2442, 1443));

  selection.primary_move_up(2);

  auto all = selection.all_locations();

  EXPECT_EQ(all.size(), 4);
  EXPECT_TRUE(all.find(Location(42, 381)) != all.end());
  EXPECT_TRUE(all.find(Location(334, 945390)) != all.end());
  EXPECT_TRUE(all.find(Location(12, 1)) != all.end());
  EXPECT_TRUE(all.find(Location(-2442, 1443)) != all.end());
}

TEST(TableCellSelectionTests, PrimaryMoveDownWorks) {
  TableCellSelection selection;

  selection.set_primary(Location(42, 383));
  selection.add_cell(Location(334, 945390));
  selection.add_cell(Location(12, 1));
  selection.add_cell(Location(-2442, 1443));

  selection.primary_move_down(2);

  auto all = selection.all_locations();

  EXPECT_EQ(all.size(), 4);
  EXPECT_TRUE(all.find(Location(42, 385)) != all.end());
  EXPECT_TRUE(all.find(Location(334, 945390)) != all.end());
  EXPECT_TRUE(all.find(Location(12, 1)) != all.end());
  EXPECT_TRUE(all.find(Location(-2442, 1443)) != all.end());
}

TEST(TableCellSelectionTests, PrimaryMoveLeftWorks) {
  TableCellSelection selection;

  selection.set_primary(Location(42, 383));
  selection.add_cell(Location(334, 945390));
  selection.add_cell(Location(12, 1));
  selection.add_cell(Location(-2442, 1443));

  selection.primary_move_left();

  auto all = selection.all_locations();

  EXPECT_EQ(all.size(), 4);
  EXPECT_TRUE(all.find(Location(41, 383)) != all.end());
  EXPECT_TRUE(all.find(Location(334, 945390)) != all.end());
  EXPECT_TRUE(all.find(Location(12, 1)) != all.end());
  EXPECT_TRUE(all.find(Location(-2442, 1443)) != all.end());
}

TEST(TableCellSelectionTests, PrimaryMoveRightWorks) {
  TableCellSelection selection;

  selection.set_primary(Location(42, 383));
  selection.add_cell(Location(334, 945390));
  selection.add_cell(Location(12, 1));
  selection.add_cell(Location(-2442, 1443));

  selection.primary_move_right();

  auto all = selection.all_locations();

  EXPECT_EQ(all.size(), 4);
  EXPECT_TRUE(all.find(Location(43, 383)) != all.end());
  EXPECT_TRUE(all.find(Location(334, 945390)) != all.end());
  EXPECT_TRUE(all.find(Location(12, 1)) != all.end());
  EXPECT_TRUE(all.find(Location(-2442, 1443)) != all.end());
}
