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
  Location() {
    x_ = 0;
    y_ = 0;
  }

  explicit Location(long x, long y) {
    x_ = x;
    y_ = y;
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
