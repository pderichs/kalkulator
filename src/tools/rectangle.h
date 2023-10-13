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

#ifndef RECTANGLE_INCLUDED
#define RECTANGLE_INCLUDED

#include "location.h"
#include <stdexcept>

class Rectangle {
private:
  Location topLeft_;
  Location bottomRight_;

public:
  Rectangle(const Location &topLeft, const Location &bottomRight)
      : topLeft_(topLeft), bottomRight_(bottomRight) {
    if (!(topLeft_ < bottomRight_)) {
      throw std::runtime_error("This is not a rect");
    }
  }

  bool contains(const Location &location) const {
    if (location.x() < left()) {
      return false;
    }

    if (location.x() > right()) {
      return false;
    }

    if (location.y() < top()) {
      return false;
    }

    if (location.y() > bottom()) {
      return false;
    }

    return true;
  }

  int left() const { return topLeft_.x(); }

  int right() const { return bottomRight_.x(); }

  int top() const { return topLeft_.y(); }

  int bottom() const { return bottomRight_.y(); }

  int width() const { return right() - left(); }

  int height() const { return bottom() - top(); }

  void enlarge_all_directions(int amount) {
    if (amount <= 0) {
      return;
    }

    topLeft_ = Location(topLeft_.x() - amount, topLeft_.y() - amount);
    bottomRight_ =
        Location(bottomRight_.x() + amount, bottomRight_.y() + amount);
  }
};

#endif
