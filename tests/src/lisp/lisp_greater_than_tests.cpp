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

TEST(LispGreaterThanTests, GreaterThanTests1) {
 std::map<std::string, LispValuePtr> tests = {
     {"(> 1 0)", LispValueFactory::new_bool(LISP_BOOL_TRUE)},
     {"(> 0 1)", LispValueFactory::new_bool(LISP_BOOL_FALSE)},
     {"(> (+ 1 2) (+ 2 3))", LispValueFactory::new_bool(LISP_BOOL_FALSE)},
     {"(> (+ 2 3) (+ 1 0))", LispValueFactory::new_bool(LISP_BOOL_TRUE)},
     {"(> 1 1)", LispValueFactory::new_bool(LISP_BOOL_FALSE)},
 };

 return execute_lisp_tests(tests, ">");
}

TEST(LispGreaterThanTests, GreaterThanFailsWithWrongParameterCountMoreParams) {
 LispParser parser("(> 19 -32 6)");

 LispTokens tokens;
 EXPECT_NO_THROW(tokens = parser.parse());

 LispValueParser value_parser(tokens);

 auto value = value_parser.next();
 EXPECT_TRUE(value);

 LispExecutionContext executor;
 LispValuePtr result = executor.execute(value, {});

 EXPECT_EQ(*result, "#PARAMCOUNTERR");
}

TEST(LispGreaterThanTests, GreaterThanFailsWithWrongParameterCountLessParams) {
  LispParser parser("(> 19)");

  LispTokens tokens;
  EXPECT_NO_THROW(tokens = parser.parse());

  LispValueParser value_parser(tokens);

  auto value = value_parser.next();
  EXPECT_TRUE(value);

  LispExecutionContext executor;
  LispValuePtr result = executor.execute(value, {});

  EXPECT_EQ(*result, "#PARAMCOUNTERR");
}

TEST(LispGreaterThanTests, GreaterThanFailsWithWrongParameterTypesFirst) {
  LispParser parser("(> \"Hello\" 1)");

  LispTokens tokens;
  EXPECT_NO_THROW(tokens = parser.parse());

  LispValueParser value_parser(tokens);

  auto value = value_parser.next();
  EXPECT_TRUE(value);

  LispExecutionContext executor;
  LispValuePtr result = executor.execute(value, {});

  EXPECT_EQ(*result, "#PARAMERR");
}

TEST(LispGreaterThanTests, GreaterThanFailsWithWrongParameterTypesSecond) {
  LispParser parser("(> 1 \"Hello\")");

  LispTokens tokens;
  EXPECT_NO_THROW(tokens = parser.parse());

  LispValueParser value_parser(tokens);

  auto value = value_parser.next();
  EXPECT_TRUE(value);

  LispExecutionContext executor;
  LispValuePtr result = executor.execute(value, {});

  EXPECT_EQ(*result, "#PARAMERR");
}
