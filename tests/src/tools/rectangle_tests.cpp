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
#include "rectangle.h"

TEST(RectangleTests, RectangleHasExpectedCoordinates) {
  Rectangle rect(Location(5, 7), Location(100, 200));

  EXPECT_EQ(rect.top(), 7);
  EXPECT_EQ(rect.bottom(), 200);
  EXPECT_EQ(rect.left(), 5);
  EXPECT_EQ(rect.right(), 100);
}

TEST(RectangleTests, WidthWorks) {
  Rectangle rect(Location(5, 7), Location(100, 200));

  EXPECT_EQ(rect.width(), 95);
}

TEST(RectangleTests, HeightWorks) {
  Rectangle rect(Location(5, 7), Location(100, 200));

  EXPECT_EQ(rect.height(), 193);
}

TEST(RectangleTests, Contains) {
  Rectangle rect(Location(5, 7), Location(100, 200));

  EXPECT_TRUE(rect.contains(Location(5, 7)));
  EXPECT_FALSE(rect.contains(Location(4, 7)));
  EXPECT_FALSE(rect.contains(Location(5, 6)));

  EXPECT_TRUE(rect.contains(Location(100, 200)));
  EXPECT_FALSE(rect.contains(Location(101, 200)));
  EXPECT_FALSE(rect.contains(Location(100, 201)));
}

TEST(RectangleTests, ExtendAllDirections) {
  Rectangle rect(Location(5, 7), Location(100, 200));

  rect.extend_all_directions(1);

  EXPECT_EQ(rect.top(), 6);
  EXPECT_EQ(rect.bottom(), 201);
  EXPECT_EQ(rect.left(), 4);
  EXPECT_EQ(rect.right(), 101);
}

TEST(RectangleTests, ExtendUp) {
  Rectangle rect(Location(5, 7), Location(100, 200));

  rect.extend_up();

  EXPECT_EQ(rect.top(), 6);
  EXPECT_EQ(rect.bottom(), 200);
  EXPECT_EQ(rect.left(), 5);
  EXPECT_EQ(rect.right(), 100);
}

TEST(RectangleTests, ExtendDown) {
  Rectangle rect(Location(5, 7), Location(100, 200));

  rect.extend_down();

  EXPECT_EQ(rect.top(), 7);
  EXPECT_EQ(rect.bottom(), 201);
  EXPECT_EQ(rect.left(), 5);
  EXPECT_EQ(rect.right(), 100);
}

TEST(RectangleTests, ExtendLeft) {
  Rectangle rect(Location(5, 7), Location(100, 200));

  rect.extend_left();

  EXPECT_EQ(rect.top(), 7);
  EXPECT_EQ(rect.bottom(), 200);
  EXPECT_EQ(rect.left(), 4);
  EXPECT_EQ(rect.right(), 100);
}

TEST(RectangleTests, ExtendRight) {
  Rectangle rect(Location(5, 7), Location(100, 200));

  rect.extend_right();

  EXPECT_EQ(rect.top(), 7);
  EXPECT_EQ(rect.bottom(), 200);
  EXPECT_EQ(rect.left(), 5);
  EXPECT_EQ(rect.right(), 101);
}

TEST(RectangleTests, AllLocations) {
  Rectangle rect(Location(0, 0), Location(2, 2));

  auto locations = rect.all_locations();

  EXPECT_EQ(locations.size(), 9);

  EXPECT_TRUE(locations.find(Location(0, 0)) != locations.end());
  EXPECT_TRUE(locations.find(Location(0, 1)) != locations.end());
  EXPECT_TRUE(locations.find(Location(0, 2)) != locations.end());
  EXPECT_TRUE(locations.find(Location(1, 0)) != locations.end());
  EXPECT_TRUE(locations.find(Location(1, 1)) != locations.end());
  EXPECT_TRUE(locations.find(Location(1, 2)) != locations.end());
  EXPECT_TRUE(locations.find(Location(2, 0)) != locations.end());
  EXPECT_TRUE(locations.find(Location(2, 1)) != locations.end());
  EXPECT_TRUE(locations.find(Location(2, 2)) != locations.end());

  // Also behaves correctly on change
  rect.extend_right();

  locations = rect.all_locations();

  EXPECT_EQ(locations.size(), 12);

  EXPECT_TRUE(locations.find(Location(0, 0)) != locations.end());
  EXPECT_TRUE(locations.find(Location(0, 1)) != locations.end());
  EXPECT_TRUE(locations.find(Location(0, 2)) != locations.end());
  EXPECT_TRUE(locations.find(Location(1, 0)) != locations.end());
  EXPECT_TRUE(locations.find(Location(1, 1)) != locations.end());
  EXPECT_TRUE(locations.find(Location(1, 2)) != locations.end());
  EXPECT_TRUE(locations.find(Location(2, 0)) != locations.end());
  EXPECT_TRUE(locations.find(Location(2, 1)) != locations.end());
  EXPECT_TRUE(locations.find(Location(2, 2)) != locations.end());
  EXPECT_TRUE(locations.find(Location(3, 0)) != locations.end());
  EXPECT_TRUE(locations.find(Location(3, 1)) != locations.end());
  EXPECT_TRUE(locations.find(Location(3, 2)) != locations.end());
}