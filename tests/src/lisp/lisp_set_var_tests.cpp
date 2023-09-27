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

TEST(LispSetVarTests, SetVarTest1) {
  std::map<std::string, LispValuePtr> tests = {
      {"(set-var test1 0)", LispValueFactory::new_integer(0)},
      {"(set-var test1 -5)", LispValueFactory::new_integer(-5)},
      {"(set-var test1 -15.377)", LispValueFactory::new_double(-15.377)},
      {"(set-var test1 18.847)", LispValueFactory::new_double(18.847)},
      {"(set-var test1 \"Hello\")", LispValueFactory::new_string("Hello")},
  };

  return execute_lisp_tests(tests, "set-var");
}

TEST(LispSetVarTests, SetVarDoesNotAcceptWrongParameterFormat) {
  LispParser parser("(set-var 1 \"Hello\")");

  LispTokens tokens;
  EXPECT_NO_THROW(tokens = parser.parse());

  LispValueParser value_parser(tokens);

  auto value = value_parser.next();
  EXPECT_TRUE(value);

  LispExecutionContext executor;
  LispValuePtr result = executor.execute(value, {});

  EXPECT_EQ(*result, "#PARAMERR");
}

TEST(LispSetVarTests, SetVarDoesNotAcceptWrongParameterCountLess) {
  LispParser parser("(set-var)");

  LispTokens tokens;
  EXPECT_NO_THROW(tokens = parser.parse());

  LispValueParser value_parser(tokens);

  auto value = value_parser.next();
  EXPECT_TRUE(value);

  LispExecutionContext executor;
  LispValuePtr result = executor.execute(value, {});

  EXPECT_EQ(*result, "#PARAMCOUNTERR");
}

TEST(LispSetVarTests, SetVarDoesNotAcceptWrongParameterCountOne) {
  LispParser parser("(set-var hello)");

  LispTokens tokens;
  EXPECT_NO_THROW(tokens = parser.parse());

  LispValueParser value_parser(tokens);

  auto value = value_parser.next();
  EXPECT_TRUE(value);

  LispExecutionContext executor;
  LispValuePtr result = executor.execute(value, {});

  EXPECT_EQ(*result, "#PARAMCOUNTERR");
}

