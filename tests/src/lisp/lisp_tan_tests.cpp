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

TEST(LispTanTests, TanTest1) {
  std::map<std::string, LispValuePtr> tests = {
      {"(tan 0)", LispValueFactory::new_double(std::tan(0))},
      {"(tan 45)", LispValueFactory::new_double(std::tan(45))},
      {"(tan 15)", LispValueFactory::new_double(std::tan(15))},
      {"(tan 18)", LispValueFactory::new_double(std::tan(18))},
  };

  return execute_lisp_tests(tests, "tan");
}

TEST(LispTanTests, TanDoesNotAcceptWrongParameterCountMore) {
  LispParser parser("(tan 1 \"Hello\")");

  LispTokens tokens;
  EXPECT_NO_THROW(tokens = parser.parse());

  LispValueParser value_parser(tokens);

  auto value = value_parser.next();
  EXPECT_TRUE(value);

  LispExecutionContext executor;
  LispValuePtr result = executor.execute(value, {});

  EXPECT_EQ(*result, "#PARAMCOUNTERR");
}

TEST(LispTanTests, TanDoesNotAcceptWrongParameterCountLess) {
  LispParser parser("(tan)");

  LispTokens tokens;
  EXPECT_NO_THROW(tokens = parser.parse());

  LispValueParser value_parser(tokens);

  auto value = value_parser.next();
  EXPECT_TRUE(value);

  LispExecutionContext executor;
  LispValuePtr result = executor.execute(value, {});

  EXPECT_EQ(*result, "#PARAMCOUNTERR");
}

TEST(LispTanTests, TanDoesNotAcceptWrongParameterFormat) {
  LispParser parser("(tan \"Hello\")");

  LispTokens tokens;
  EXPECT_NO_THROW(tokens = parser.parse());

  LispValueParser value_parser(tokens);

  auto value = value_parser.next();
  EXPECT_TRUE(value);

  LispExecutionContext executor;
  LispValuePtr result = executor.execute(value, {});

  EXPECT_EQ(*result, "#PARAMERR");
}
