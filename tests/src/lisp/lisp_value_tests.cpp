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
#include <memory>
#include <tuple>
#include <vector>
#include "model/lisp/lisp_value_factory.h"
#include "lisp/lisp_common_values.h"

TEST(LispValueTests, LispValueEqualityTests) {
  // clang-format off
  std::vector<std::pair<std::pair<LispValuePtr, LispValuePtr>, bool>> tests = {
      {{LispValueFactory::new_double(42), LispValueFactory::new_double(42)}, true},
      {{LispValueFactory::new_double(43), LispValueFactory::new_double(42)}, false},
      {{LispValueFactory::new_double(42), LispValueFactory::new_double(41)}, false},
      {{LispValueFactory::new_string("hello"), LispValueFactory::new_double(42)}, false},
      {{LispValueFactory::new_string("hello"), LispValueFactory::new_string("hello")}, true},
      {{LispValueFactory::new_string("hello"), LispValueFactory::new_string("hallo")}, false},
      {{LispCommonValues::true_value(), LispCommonValues::true_value()}, true},
      {{LispCommonValues::false_value(), LispCommonValues::true_value()}, false},
      {{LispCommonValues::true_value(), LispCommonValues::false_value()}, false},
      {{LispCommonValues::false_value(), LispCommonValues::false_value()}, true},

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
    EXPECT_EQ(result, expected_result);
  }
}
