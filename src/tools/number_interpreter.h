#ifndef NUMBER_INTERPRETER_INCLUDED
#define NUMBER_INTERPRETER_INCLUDED

#include <optional>
#include <sstream>
#include <string>

class NumberInterpreter {
public:
  static std::optional<double> to_double(const std::string& str) {
    std::istringstream iss(str);
    double value;
    char leftover;
    bool is_double = (iss >> value) && !(iss >> leftover);
    if (!is_double) {
      return {};
    }

    return value;
  }

  static std::optional<int64_t> to_integer(const std::string& str) {
    std::istringstream iss(str);
    int64_t value;
    char leftover;
    bool is_int = (iss >> value) && !(iss >> leftover);

    if (!is_int) {
      return {};
    }

    return value;
  }
};

#endif
