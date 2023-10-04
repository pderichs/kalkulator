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

#include "lisp/lisp_common_values.h"
#include "lisp/lisp_execution_context.h"
#include "lisp/lisp_parser.h"
#include "lisp/lisp_value_parser.h"
#include "model/lisp/lisp_execution_context_error.h"
#include "model/lisp/lisp_value_factory.h"
#include "gtest/gtest.h"

TEST(LispDefunTests, DefunTest1) {
  LispParser parser(
      "(defun test-function1 (x y) \"Some test function\" (print (+ x y)))");

  LispTokens tokens;
  EXPECT_NO_THROW(tokens = parser.parse());

  LispValueParser value_parser(tokens);

  auto value = value_parser.next();
  EXPECT_TRUE(value);

  LispExecutionContext executor;
  LispValuePtr result = executor.execute(value, {}, 0);

  auto function_definition_val = executor.scope_variable("test-function1");
  EXPECT_TRUE(function_definition_val->is_function_definition());

  auto function_definition = function_definition_val->function_definition();
  EXPECT_EQ(function_definition.name, "test-function1");

  EXPECT_EQ(function_definition.comment, "Some test function");

  EXPECT_EQ(function_definition.parameter_definitions.size(), 2);
  EXPECT_EQ(function_definition.parameter_definitions[0], "x");
  EXPECT_EQ(function_definition.parameter_definitions[1], "y");

  EXPECT_EQ(function_definition.body.size(), 2);
}

TEST(LispDefunTests, DefunTestFailsIfFunctionNameHasWrongFormat) {
  LispParser parser("(defun 1 (x y) \"Some test function\" (print (+ x y)))");

  LispTokens tokens;
  EXPECT_NO_THROW(tokens = parser.parse());

  LispValueParser value_parser(tokens);

  auto value = value_parser.next();
  EXPECT_TRUE(value);

  LispExecutionContext executor;
  LispValuePtr result = executor.execute(value, {}, 0);

  EXPECT_TRUE(result->is_error());
  EXPECT_EQ(result->string(), "#PARAMERR");
}

TEST(LispDefunTests, DefunTestFailsIfFunctionParamsHaveWrongFormat) {
  LispParser parser("(defun a1 wrong \"Some test function\" (print (+ x y)))");

  LispTokens tokens;
  EXPECT_NO_THROW(tokens = parser.parse());

  LispValueParser value_parser(tokens);

  auto value = value_parser.next();
  EXPECT_TRUE(value);

  LispExecutionContext executor;
  LispValuePtr result = executor.execute(value, {}, 0);

  EXPECT_TRUE(result->is_error());
  EXPECT_EQ(result->string(), "#PARAMERR");
}

TEST(LispDefunTests, DefunTestFailsIfFunctionCommentHasWrongFormat) {
  LispParser parser("(defun a1 (x y) 1 (print (+ x y)))");

  LispTokens tokens;
  EXPECT_NO_THROW(tokens = parser.parse());

  LispValueParser value_parser(tokens);

  auto value = value_parser.next();
  EXPECT_TRUE(value);

  LispExecutionContext executor;
  LispValuePtr result = executor.execute(value, {}, 0);

  EXPECT_TRUE(result->is_error());
  EXPECT_EQ(result->string(), "#PARAMERR");
}

TEST(LispDefunTests, DefunTestFailsIfFunctionBodyHasWrongFormat) {
  LispParser parser("(defun a1 (x y) \"comment\" 1)");

  LispTokens tokens;
  EXPECT_NO_THROW(tokens = parser.parse());

  LispValueParser value_parser(tokens);

  auto value = value_parser.next();
  EXPECT_TRUE(value);

  LispExecutionContext executor;
  LispValuePtr result = executor.execute(value, {}, 0);

  EXPECT_TRUE(result->is_error());
  EXPECT_EQ(result->string(), "#PARAMERR");
}

TEST(LispDefunTests, DefunTestFailsIfParameterCountIsWrong) {
  LispParser parser("(defun)");

  LispTokens tokens;
  EXPECT_NO_THROW(tokens = parser.parse());

  LispValueParser value_parser(tokens);

  auto value = value_parser.next();
  EXPECT_TRUE(value);

  LispExecutionContext executor;
  LispValuePtr result = executor.execute(value, {}, 0);

  EXPECT_TRUE(result->is_error());
  EXPECT_EQ(result->string(), "#PARAMCOUNTERR");
}
