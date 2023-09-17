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


#ifndef LOCATION_INCLUDED
#define LOCATION_INCLUDED

#include <cmath>
#include <set>
#include <vector>

// A simple coordinate class. Combines x and y.
class Location {
private:
  long x_, y_;

public:
  Location(): x_(0), y_(0) {
  }

  explicit Location(long x, long y): x_(x), y_(y) {
  }

  long x() const { return x_; }
  long y() const { return y_; }

  void moveLeft() { x_--; }
  void moveRight() { x_++; }
  void moveDown() { y_++; }
  void moveUp() { y_--; }

  void moveUp(int amount) { y_ -= amount; }
  void moveDown(int amount) { y_ += amount; }

  bool operator==(const Location &other) const {
    return x_ == other.x_ && y_ == other.y_;
  }

  bool operator!=(const Location &other) const {
    return x_ != other.x_ || y_ != other.y_;
  }

  Location operator+(const Location &other) const {
    return Location(x_ + other.x_, y_ + other.y_);
  };

  // Operator to be used for maps and sets
  // TODO Must be tested thoroughly!
  bool operator<(const Location &other) const {
    return (x_ < other.x_) || (x_ == other.x_ && y_ < other.y_);
  }

  double distance_to(const Location &other) const {
    return std::abs(x_ - other.x()) + std::abs(y_ - other.y());
  }
};

typedef std::vector<Location> LocationVector;
typedef std::set<Location> LocationSet;

#endif
