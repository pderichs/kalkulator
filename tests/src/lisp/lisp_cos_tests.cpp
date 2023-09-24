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

TEST(LispCosTests, CosTest1) {
  std::map<std::string, LispValuePtr> tests = {
      {"(cos 0)", LispValueFactory::new_double(std::cos(0))},
      {"(cos 45)", LispValueFactory::new_double(std::cos(45))},
      {"(cos 15)", LispValueFactory::new_double(std::cos(15))},
      {"(cos 18)", LispValueFactory::new_double(std::cos(18))},
  };

  return execute_lisp_tests(tests, "cos");
}

TEST(LispCosTests, CosDoesNotAcceptWrongParameterCountMore) {
  LispParser parser("(cos 1 \"Hello\")");

  LispTokens tokens;
  EXPECT_NO_THROW(tokens = parser.parse());

  LispValueParser value_parser(tokens);

  auto value = value_parser.next();
  EXPECT_TRUE(value);

  LispExecutionContext executor;
  LispValuePtr result = executor.execute(value, {});

  EXPECT_EQ(*result, "#PARAMCOUNTERR");
}

TEST(LispCosTests, CosDoesNotAcceptWrongParameterCountLess) {
  LispParser parser("(cos)");

  LispTokens tokens;
  EXPECT_NO_THROW(tokens = parser.parse());

  LispValueParser value_parser(tokens);

  auto value = value_parser.next();
  EXPECT_TRUE(value);

  LispExecutionContext executor;
  LispValuePtr result = executor.execute(value, {});

  EXPECT_EQ(*result, "#PARAMCOUNTERR");
}

TEST(LispCosTests, CosDoesNotAcceptWrongParameterFormat) {
  LispParser parser("(cos \"Hello\")");

  LispTokens tokens;
  EXPECT_NO_THROW(tokens = parser.parse());

  LispValueParser value_parser(tokens);

  auto value = value_parser.next();
  EXPECT_TRUE(value);

  LispExecutionContext executor;
  LispValuePtr result = executor.execute(value, {});

  EXPECT_EQ(*result, "#PARAMERR");
}
