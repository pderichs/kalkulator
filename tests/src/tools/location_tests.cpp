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
#include "location.h"

TEST(LocationTests, InitialLocationIsAtX0Y0) {
  Location location;

  EXPECT_EQ(location.x(), 0);
  EXPECT_EQ(location.y(), 0);
}

TEST(LocationTests, LocationAcceptsCoordinates) {
  Location location(10, 42);

  EXPECT_EQ(location.x(), 10);
  EXPECT_EQ(location.y(), 42);
}

TEST(LocationTests, LocationLowerThanRespectsX) {
  Location location1(9, 42);
  Location location2(10, 42);

  EXPECT_TRUE(location1 < location2);
}

TEST(LocationTests, LocationLowerThanRespectsY) {
  Location location1(10, 42);
  Location location2(10, 43);

  EXPECT_TRUE(location1 < location2);
}

TEST(LocationTests, LocationLowerThanEquality) {
  Location location1(10, 42);
  Location location2(10, 42);

  EXPECT_FALSE(location1 < location2);
}

TEST(LocationTests, LocationLowerThanMismatchX) {
  Location location1(11, 42);
  Location location2(10, 42);

  EXPECT_FALSE(location1 < location2);
}

TEST(LocationTests, LocationLowerThanMismatchY) {
  Location location1(10, 43);
  Location location2(10, 42);

  EXPECT_FALSE(location1 < location2);
}

TEST(LocationTests, LocationEqualityNotEqualX) {
  Location location1(10, 42);
  Location location2(11, 42);

  EXPECT_FALSE(location1 == location2);
}

TEST(LocationTests, LocationEqualityNotEqualY) {
  Location location1(10, 43);
  Location location2(10, 42);

  EXPECT_FALSE(location1 == location2);
}

TEST(LocationTests, LocationEqualityEqual) {
  Location location1(10, 42);
  Location location2(10, 42);

  EXPECT_TRUE(location1 == location2);
}

TEST(LocationTests, LocationInEqualityNotEqualX) {
  Location location1(10, 42);
  Location location2(11, 42);

  EXPECT_TRUE(location1 != location2);
}

TEST(LocationTests, LocationInEqualityNotEqualY) {
  Location location1(10, 43);
  Location location2(10, 42);

  EXPECT_TRUE(location1 != location2);
}

TEST(LocationTests, LocationInEqualityEqual) {
  Location location1(10, 42);
  Location location2(10, 42);

  EXPECT_FALSE(location1 != location2);
}

TEST(LocationTests, Addition) {
  Location location1(10, 42);
  Location location2(1, 2);

  auto result = location1 + location2;

  EXPECT_EQ(result, Location(11, 44));
}