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

#include "model/lisp/lisp_execution_context_error.h"
#include "gtest/gtest.h"
#include "lisp/lisp_value_factory.h"
#include "lisp_test_tools.h"

TEST(LispAbsTests, AbsTest1) {
  std::map<std::string, LispValuePtr> tests = {
      {"(abs 0)", LispValueFactory::new_double(std::abs(0))},
      {"(abs -5)", LispValueFactory::new_double(std::abs(-5))},
      {"(abs -15.377)", LispValueFactory::new_double(std::abs(-15.377))},
      {"(abs 18.847)", LispValueFactory::new_double(std::abs(18.847))},
      {"(abs 1 \"Hello\")", LispValueFactory::new_error("#PARAMCOUNTERR")},
      {"(abs)", LispValueFactory::new_error("#PARAMCOUNTERR")},
      {"(abs \"Hello\")", LispValueFactory::new_error("#PARAMERR")},
  };

  return execute_lisp_tests(tests, "abs");
}
