#include <functional>
#include <memory>
#include <tuple>
#include <vector>

#include "../../model/lisp/lisp_value.h"
#include "../../model/lisp/lisp_value_factory.h"
#include "tools.h"

int run_lisp_value_equality_tests();

template <typename... Args> LispValuePtrVector make_value_list(Args... args);

int run_lisp_value_tests() {
  RUN_TEST(run_lisp_value_equality_tests);
  return 0;
}

int run_lisp_value_equality_tests() {
  // clang-format off
  std::vector<std::pair<std::pair<LispValuePtr, LispValuePtr>, bool>> tests = {
      {{LispValueFactory::new_double(42), LispValueFactory::new_double(42)}, true},
      {{LispValueFactory::new_double(43), LispValueFactory::new_double(42)}, false},
      {{LispValueFactory::new_double(42), LispValueFactory::new_double(41)}, false},
      {{LispValueFactory::new_string("hello"), LispValueFactory::new_double(42)}, false},
      {{LispValueFactory::new_string("hello"), LispValueFactory::new_string("hello")}, true},
      {{LispValueFactory::new_string("hello"), LispValueFactory::new_string("hallo")}, false},
      {{LispValueFactory::new_bool(LISP_BOOL_TRUE), LispValueFactory::new_bool(LISP_BOOL_TRUE)}, true},
      {{LispValueFactory::new_bool(LISP_BOOL_FALSE), LispValueFactory::new_bool(LISP_BOOL_TRUE)}, false},
      {{LispValueFactory::new_bool(LISP_BOOL_TRUE), LispValueFactory::new_bool(LISP_BOOL_FALSE)}, false},
      {{LispValueFactory::new_bool(LISP_BOOL_FALSE), LispValueFactory::new_bool(LISP_BOOL_FALSE)}, true},

      {{LispValueFactory::new_list({LispValueFactory::new_double(10), LispValueFactory::new_double(42), LispValueFactory::new_double(-1999)}), LispValueFactory::new_list({LispValueFactory::new_double(10), LispValueFactory::new_double(42), LispValueFactory::new_double(-1999)})}, true},
      {{LispValueFactory::new_list({LispValueFactory::new_double(11), LispValueFactory::new_double(42), LispValueFactory::new_double(-1999)}), LispValueFactory::new_list({LispValueFactory::new_double(10), LispValueFactory::new_double(42), LispValueFactory::new_double(-1999)})}, false},
      {{LispValueFactory::new_list({LispValueFactory::new_double(10), LispValueFactory::new_double(42), LispValueFactory::new_double(-1999)}), LispValueFactory::new_list({LispValueFactory::new_double(10), LispValueFactory::new_double(43), LispValueFactory::new_double(-1999)})}, false},
      {{LispValueFactory::new_list({LispValueFactory::new_double(10), LispValueFactory::new_string("hello"), LispValueFactory::new_double(-1999)}), LispValueFactory::new_list({LispValueFactory::new_double(10), LispValueFactory::new_double(43), LispValueFactory::new_double(-1999)})}, false},
      {{LispValueFactory::new_list({LispValueFactory::new_double(10), LispValueFactory::new_double(42), LispValueFactory::new_double(-1999)}), LispValueFactory::new_list({LispValueFactory::new_double(10), LispValueFactory::new_string("hello"), LispValueFactory::new_double(-1999)})}, false},
      {{LispValueFactory::new_list({LispValueFactory::new_double(10), LispValueFactory::new_string("test"), LispValueFactory::new_double(-1999)}), LispValueFactory::new_list({LispValueFactory::new_double(10), LispValueFactory::new_string("test"), LispValueFactory::new_double(-1999)})}, true},
      {{LispValueFactory::new_list({LispValueFactory::new_double(42), LispValueFactory::new_double(10), LispValueFactory::new_double(-1999)}), LispValueFactory::new_list({LispValueFactory::new_double(10), LispValueFactory::new_double(42), LispValueFactory::new_double(-1999)})}, false},
  };
  // clang-format on

  for (const auto &it : tests) {
    const auto &pair = it.first;
    const auto &expected_result = it.second;

    bool result = *pair.first == *pair.second;
    TEST_ASSERT(result == expected_result);
  }

  return 0;
}
