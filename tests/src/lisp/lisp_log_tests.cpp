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
#include "lisp/lisp_value_factory.h"
#include "lisp_test_tools.h"
#include "lisp/lisp_common_values.h"

TEST(LispLogTests, LogTest1) {
  std::map<std::string, LispValuePtr> tests = {
      {"(log 0)", LispValueFactory::new_double(std::log(0))},
      {"(log 5)", LispValueFactory::new_double(std::log(5))},
      {"(log 15.377)", LispValueFactory::new_double(std::log(15.377))},
      {"(log 18.847)", LispValueFactory::new_double(std::log(18.847))},
      {"(log 1 \"Hello\")", LispCommonValues::error_parameter_count()},
      {"(log)", LispCommonValues::error_parameter_count()},
      {"(log \"Hello\")", LispCommonValues::error_parameter()},
  };

  return execute_lisp_tests(tests, "log");
}

