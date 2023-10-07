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
#include "lisp/lisp_common_values.h"
#include "lisp_test_tools.h"

TEST(LispIsNoneTests, IsNoneTest1) {
  std::map<std::string, LispValuePtr> tests = {
      {"(is-none 0)", LispCommonValues::false_value()},
      {"(is-none 1)", LispCommonValues::false_value()},
      {"(is-none 0 1)", LispCommonValues::false_value()},
      {"(is-none some-identifier)", LispCommonValues::false_value()},
      {"(is-none \"Hello\")", LispCommonValues::false_value()},
      {"(is-none nil)", LispCommonValues::true_value()},
  };

  return execute_lisp_tests(tests, "is-none");
}

