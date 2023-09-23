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

TEST(LispValueTests, LispValueTests) {
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

  for (const auto &it : tests) {
    const auto &pair = it.first;
    const auto &expected_result = it.second;

    bool result = *pair.first == *pair.second;
    EXPECT_EQ(result, expected_result);
  }
}
