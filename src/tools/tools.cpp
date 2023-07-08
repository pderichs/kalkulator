#include "tools.h"
#include <algorithm>
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

  oss << "(t:" << rect.top() << ", l:" << rect.left()
      << ", b:" << rect.bottom() << ", r:" << rect.right() << ")";

  return oss.str();
}

}; // namespace pdtools
