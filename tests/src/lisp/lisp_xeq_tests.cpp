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
#include "gtest/gtest.h"
#include "lisp/lisp_common_values.h"

TEST(LispXeqTests, XeqTest1) {
  std::map<std::string, LispValuePtr> tests = {
      {"(xeq 1 1)", LispCommonValues::true_value()},
      {"(xeq \"Hase\" 1)", LispCommonValues::false_value()},
      {"(xeq \"Hase\" \"Bär\")", LispCommonValues::false_value()},
      {R"((xeq "Hase" "Hase"))", LispCommonValues::true_value()},
      {"(xeq (list 54 2 66 9.326) (list 54 2 66 9.326))", LispCommonValues::false_value()},
      {"(xeq (+ 54 2 66 9.3265) (+ 54 2 66 9.3265))", LispCommonValues::true_value()},
      {"(xeq (list 6 6 6) (list 6 6 6) 6)", LispCommonValues::true_value()},
      {"(xeq (list 6 6 6) (list 6 6 6) 5)", LispCommonValues::false_value()}};

  return execute_lisp_tests(tests, "xeq");
}