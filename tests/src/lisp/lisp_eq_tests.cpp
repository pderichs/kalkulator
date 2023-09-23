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

#include "model/lisp/lisp_execution_context.h"
#include "model/lisp/lisp_value_factory.h"
#include "lisp_test_tools.h"
#include "gtest/gtest.h"

TEST(LispEqTests, EqTest1) {
  std::map<std::string, LispValuePtr> tests = {
      {"(eq 1 1)", LispValueFactory::new_bool(LISP_BOOL_TRUE)},
      {"(eq \"Hase\" 1)", LispValueFactory::new_bool(LISP_BOOL_FALSE)},
      {"(eq \"Hase\" \"Bär\")", LispValueFactory::new_bool(LISP_BOOL_FALSE)},
      {R"((eq "Hase" "Hase"))", LispValueFactory::new_bool(LISP_BOOL_TRUE)},
      {"(eq (list 54 2 66 9.326) (list 54 2 66 9.326))", LispValueFactory::new_bool(LISP_BOOL_TRUE)},
      {"(eq (list 54 2 66 9.326) (list 54 3 66 9.326))", LispValueFactory::new_bool(LISP_BOOL_FALSE)},
      {"(eq (+ 54 2 66 9.3265) (+ 54 2 66 9.3265))", LispValueFactory::new_bool(LISP_BOOL_TRUE)},
      {"(eq (list 6 6 6) (list 6 6 6) 6)", LispValueFactory::new_bool(LISP_BOOL_FALSE)},
      {"(eq (list 6 6 6) (list 6 6 6) 5)", LispValueFactory::new_bool(LISP_BOOL_FALSE)}};

  return execute_lisp_tests(tests, "eq");
}
