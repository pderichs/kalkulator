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
#include "model/lisp/lisp_execution_context_error.h"
#include "model/lisp/lisp_function_execution_context.h"
#include "model/lisp/lisp_parser.h"
#include "model/lisp/lisp_value.h"
#include "model/lisp/lisp_value_factory.h"
#include "model/lisp/lisp_value_parser.h"
#include "gtest/gtest.h"

TEST(LispSinTests, SinTest1) {
  std::map<std::string, LispValuePtr> tests = {
      {"(sin 0)", LispValueFactory::new_double(std::sin(0))},
      {"(sin 45)", LispValueFactory::new_double(std::sin(45))},
      {"(sin 15)", LispValueFactory::new_double(std::sin(15))},
      {"(sin 18)", LispValueFactory::new_double(std::sin(18))},
      {"(sin 1 \"Hello\")", LispValueFactory::new_error("#PARAMCOUNTERR")},
      {"(sin)", LispValueFactory::new_error("#PARAMCOUNTERR")},
      {"(sin \"Hello\")", LispValueFactory::new_error("#PARAMERR")},
  };

  return execute_lisp_tests(tests, "sin");
}
