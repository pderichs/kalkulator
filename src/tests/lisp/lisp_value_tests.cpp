#include <vector>
#include <functional>
#include <tuple>

#include "../../model/lisp/lisp_value.h"
#include "tools.h"

int run_lisp_value_equality_tests();

template <typename... Args>
LispValuePtrVector make_value_list(Args... args);

int run_lisp_value_tests() {
  RUN_TEST(run_lisp_value_equality_tests);
  return 0;
}

int run_lisp_value_equality_tests() {
  std::vector<std::pair<std::pair<LispValue, LispValue>, bool>> tests = {
      {{LispValue(42), LispValue(42)}, true},
      {{LispValue(43), LispValue(42)}, false},
      {{LispValue(42), LispValue(41)}, false},
      {{LispValue("hello"), LispValue(42)}, false},
      {{LispValue("hello"), LispValue("hello")}, true},
      {{LispValue("hello"), LispValue("hallo")}, false},
      {{LispValue(true), LispValue(true)}, true},
      {{LispValue(false), LispValue(true)}, false},
      {{LispValue(true), LispValue(false)}, false},
      {{LispValue(false), LispValue(false)}, true},

      {{LispValue(make_value_list(10, 42, -1999)), LispValue(make_value_list(10, 42, -1999))}, true},
      {{LispValue(make_value_list(11, 42, -1999)), LispValue(make_value_list(10, 42, -1999))}, false},
      {{LispValue(make_value_list(10, 42, -1999)), LispValue(make_value_list(10, 43, -1999))}, false},
      {{LispValue(make_value_list(10, "hello", -1999)), LispValue(make_value_list(10, 43, -1999))}, false},
      {{LispValue(make_value_list(10, 42, -1999)), LispValue(make_value_list(10, "hello", -1999))}, false},
      {{LispValue(make_value_list(10, "test", -1999)), LispValue(make_value_list(10, "test", -1999))}, true},
      {{LispValue(make_value_list(42, 10, -1999)), LispValue(make_value_list(10, 42, -1999))}, false},
  };

  for (const auto& it: tests) {
      const auto& pair = it.first;
      const auto& expected_result = it.second;

      bool result = pair.first == pair.second;
      TEST_ASSERT(result == expected_result);
  }

  return 0;
}

template<typename T>
void add_values(LispValuePtrVector& v, T val) {
  v.push_back(std::make_shared<LispValue>(val));
}
// recursive variadic template
template<typename First, typename ... Rest>
void add_values(LispValuePtrVector& v, First first, Rest ... rest) {
  add_values(v, first);
  add_values(v, rest...);
}
template <typename... Args>
LispValuePtrVector make_value_list(Args... args) {
    LispValuePtrVector result;
    add_values(result, args...);
    return result;
}
