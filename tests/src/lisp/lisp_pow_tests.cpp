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

TEST(LispPowTests, PowTest1) {
  std::map<std::string, LispValuePtr> tests = {
      {"(pow 0 2)", LispValueFactory::new_double(std::pow(0, 2))},
      {"(pow -5 10)", LispValueFactory::new_double(std::pow(-5, 10))},
      {"(pow 5 1)", LispValueFactory::new_double(std::pow(5, 1))},
      {"(pow -15.377 3)", LispValueFactory::new_double(std::pow(-15.377, 3))},
      {"(pow 18.847 15)", LispValueFactory::new_double(std::pow(18.847, 15))},
      {"(pow \"Hello\" 1)", LispCommonValues::error_parameter()},
      {"(pow 1 \"Hello\")", LispCommonValues::error_parameter()},
      {"(pow)", LispCommonValues::error_parameter_count()},
      {"(pow 10)", LispCommonValues::error_parameter_count()},
  };

  return execute_lisp_tests(tests, "pow");
}
