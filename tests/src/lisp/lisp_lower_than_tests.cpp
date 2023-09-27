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
#include "lisp/lisp_common_values.h"

TEST(LispLowerThanTests, LowerThanTests1) {
  std::map<std::string, LispValuePtr> tests = {
      {"(< 1 0)", LispCommonValues::false_value()},
      {"(< 0 1)", LispCommonValues::true_value()},
      {"(< (+ 1 2) (+ 2 3))", LispCommonValues::true_value()},
      {"(< (+ 2 3) (+ 1 0))", LispCommonValues::false_value()},
      {"(< 1 1)", LispCommonValues::false_value()},
      {"(< 19 -32 6)", LispValueFactory::new_error("#PARAMCOUNTERR")},
      {"(< 19)", LispValueFactory::new_error("#PARAMCOUNTERR")},
      {"(< \"Hello\" 1)", LispValueFactory::new_error("#PARAMERR")},
      {"(< 1 \"Hello\")", LispValueFactory::new_error("#PARAMERR")},
  };

  return execute_lisp_tests(tests, "<");
}

