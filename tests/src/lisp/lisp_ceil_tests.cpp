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

#include "lisp_test_tools.h"
#include "mock_function_context.h"
#include "model/lisp/lisp_parser.h"
#include "model/lisp/lisp_value_parser.h"
#include "gtest/gtest.h"

TEST(LispCeilTests, CeilTest1) {
  std::map<std::string, LispValuePtr> tests = {
      {"(ceil 0)", LispValueFactory::new_double(std::ceil(0))},
      {"(ceil -5)", LispValueFactory::new_double(std::ceil(-5))},
      {"(ceil -15.377)", LispValueFactory::new_double(std::ceil(-15.377))},
      {"(ceil 18.847)", LispValueFactory::new_double(std::ceil(18.847))},
      {"(ceil 1 \"Hello\")", LispCommonValues::error_parameter_count()},
      {"(ceil)", LispCommonValues::error_parameter_count()},
      {"(ceil \"Hello\")", LispCommonValues::error_parameter()},
  };

  return execute_lisp_tests(tests, "ceil");
}
