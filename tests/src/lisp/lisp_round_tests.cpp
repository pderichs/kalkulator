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
#include "gtest/gtest.h"

TEST(LispRoundTests, RoundTest1) {
  std::map<std::string, LispValuePtr> tests = {
      {"(round 0 0)", LispValueFactory::new_double(0)},
      {"(round 1.4 1)", LispValueFactory::new_double(1.4)},
      {"(round 1.5 0)", LispValueFactory::new_double(2)},
      {"(round 1.54 1)", LispValueFactory::new_double(1.5)},
      {"(round 1.55 1)", LispValueFactory::new_double(1.6)},
      {"(round 1 \"Hello\")", LispCommonValues::error_parameter()},
      {"(round)", LispCommonValues::error_parameter_count()},
      {"(round \"Hello\")", LispCommonValues::error_parameter_count()},
  };

  return execute_lisp_tests(tests, "round");
}
