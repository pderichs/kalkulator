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
