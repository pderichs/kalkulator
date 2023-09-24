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


#include "tools.h"
#include <algorithm>
#include <random>
#include <sstream>

namespace pdtools {

// Thanks to https://stackoverflow.com/a/65404298
std::string trim(const std::string &s) {
  std::string str(s);
  str.erase(0, str.find_first_not_of("\t\n\v\f\r ")); // left trim
  str.erase(str.find_last_not_of("\t\n\v\f\r ") + 1); // right trim
  return str;
}

StringVector split(const std::string &s, char delim) {
  StringVector result;
  std::stringstream ss(s);

  std::string part;
  while (std::getline(ss, part, delim)) {
    result.push_back(trim(part));
  }

  return result;
}

std::string stringVectorToString(const StringVector &v) {
  std::stringstream ss;

  std::for_each(v.begin(), v.end(),
                [&](const auto &item) { ss << item << ","; });

  return ss.str();
}

std::string intVectorToString(const IntVector &v) {
  std::stringstream ss;

  std::for_each(v.begin(), v.end(),
                [&](const auto &item) { ss << item << ","; });

  return ss.str();
}

std::string locationToString(const Location &l) {
  std::stringstream oss;

  oss << "(x: " << l.x() << ", y: " << l.y() << ")";

  return oss.str();
}

std::string rectangleToString(const Rectangle &rect) {
  std::stringstream oss;

  oss << "(t:" << rect.top() << ", l:" << rect.left() << ", b:" << rect.bottom()
      << ", r:" << rect.right() << ")";

  return oss.str();
}

// Function to generate a random integer within a specified range [min, max]
int generate_random_int_in_range(int min, int max) {
  std::random_device rd;
  std::mt19937 gen(rd()); // Mersenne Twister random number generator
  std::uniform_int_distribution<int> dist(min, max);
  return dist(gen);
}

bool double_nearly_eq(double a, double b, double epsilon) {
  return std::fabs(a - b) <= epsilon;
}

}; // namespace pdtools
